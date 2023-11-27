#include <openssl/x509.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/ocsp.h>
#include <openssl/bn.h>
#include <openssl/trace.h>
#include <openssl/async.h>
#ifndef OPENSSL_NO_CT
#include <openssl/ct.h>
#endif

#include <string>

#include <miniscript/miniscript.h>
#include <miniscript/utilities/Console.h>

using std::string;
using std::to_string;

using miniscript::utilities::Console;

#if (SSLEAY_VERSION_NUMBER >= 0x0907000L)
#include <openssl/conf.h>
#endif

void init_openssl_library(void) {
	SSL_library_init();
	SSL_load_error_strings();
	ERR_load_crypto_strings();
	OPENSSL_config(NULL);
	/* Include <openssl/opensslconf.h> to get this define */
	#if defined (OPENSSL_THREADS)
		fprintf(stdout, "Warning: thread locking is not implemented\n");
	#endif
}

int verify_callback(int preverify, X509_STORE_CTX *x509_ctx) {
	int depth = X509_STORE_CTX_get_error_depth(x509_ctx);
	int err = X509_STORE_CTX_get_error(x509_ctx);

	X509 *cert = X509_STORE_CTX_get_current_cert(x509_ctx);
	X509_NAME *iname = cert ? X509_get_issuer_name(cert) : NULL;
	X509_NAME *sname = cert ? X509_get_subject_name(cert) : NULL;

	/*
	 print_cn_name("Issuer (cn)", iname);
	 print_cn_name("Subject (cn)", sname);
	 */

	if (depth == 0) {
		/* If depth is 0, its the server's certificate. Print the SANs too */
		/*
		 print_san_name("Subject (san)", cert);
		 */
	}

	return preverify;
}

void handleFailure() {
	int err;
	while (err = ERR_get_error()) {
		char *str = ERR_error_string(err, 0);
		if (!str)
			return;
		printf(str);
		printf("\n");
		fflush(stdout);
	}
}

int main(int argc, char *argv[]) {
	// see: https://wiki.openssl.org/index.php/SSL/TLS_Client
	#define HOST_NAME "drewke.net"
	#define HOST_PORT "443"
	#define HOST_RESOURCE "/tdme2/"

	init_openssl_library();

	long res = 1;

	SSL *ssl = NULL;
	SSL_CTX *ctx = NULL;
	BIO *bio = NULL, *out = NULL;

	init_openssl_library();

	const SSL_METHOD *method = SSLv23_method();
	if (!(NULL != method))
		handleFailure();

	ctx = SSL_CTX_new(method);
	if (!(ctx != NULL))
		handleFailure();

	/* Cannot fail ??? */
	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, verify_callback);

	/* Cannot fail ??? */
	SSL_CTX_set_verify_depth(ctx, 4);

	/* Cannot fail ??? */
	const long flags = SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION;
	SSL_CTX_set_options(ctx, flags);

	// see: https://stackoverflow.com/questions/59017890/where-is-the-certificate-file-for-ssl-ctx-load-verify-locations-in-openssl-locat
	res = SSL_CTX_load_verify_locations(ctx, "/etc/ssl/certs/ca-certificates.crt" /* truststore */, "/etc/ssl/certs/");
	if (!(1 == res))
		handleFailure();

	bio = BIO_new_ssl_connect(ctx);
	if (!(bio != NULL))
		handleFailure();

	res = BIO_set_conn_hostname(bio, HOST_NAME ":" HOST_PORT);
	if (!(1 == res))
		handleFailure();

	BIO_get_ssl(bio, &ssl);
	if (!(ssl != NULL))
		handleFailure();

	const char PREFERRED_CIPHERS[] = "HIGH:!aNULL:!kRSA:!PSK:!SRP:!MD5:!RC4";
	res = SSL_set_cipher_list(ssl, PREFERRED_CIPHERS);
	if (!(1 == res))
		handleFailure();

	res = SSL_set_tlsext_host_name(ssl, HOST_NAME);
	if (!(1 == res))
		handleFailure();

	out = BIO_new_fp(stdout, BIO_NOCLOSE);
	if (!(NULL != out))
		handleFailure();

	res = BIO_do_connect(bio);
	if (!(1 == res))
		handleFailure();

	res = BIO_do_handshake(bio);
	if (!(1 == res))
		handleFailure();

	/* Step 1: verify a server certificate was presented during the negotiation */
	X509 *cert = SSL_get_peer_certificate(ssl);
	if (cert) {
		X509_free(cert);
	} /* Free immediately */
	if (NULL == cert)
		handleFailure();

	/* Step 2: verify the result of chain verification */
	/* Verification performed according to RFC 4158    */
	res = SSL_get_verify_result(ssl);
	if (!(X509_V_OK == res))
		handleFailure();

	/* Step 3: hostname verification */
	/* An exercise left to the reader */

	BIO_puts(bio, "GET " HOST_RESOURCE " HTTP/1.1\r\n"
	"Host: " HOST_NAME "\r\n"
	"Connection: close\r\n\r\n");
	BIO_puts(out, "\n");

	int len = 0;
	do {
		char buff[1536] = { };
		len = BIO_read(bio, buff, sizeof(buff));

		if (len > 0)
			BIO_write(out, buff, len);

	} while (len > 0 || BIO_should_retry(bio));

	if (out)
		BIO_free(out);

	if (bio != NULL)
		BIO_free_all(bio);

	if (NULL != ctx)
		SSL_CTX_free(ctx);
}
