#pragma once

#include <string>

#include <miniscript/miniscript.h>
#include <miniscript/os/network/fwd-miniscript.h>
#include <miniscript/os/network/NetworkIOException.h>
#include <miniscript/os/network/NetworkSocket.h>
#include <miniscript/os/network/NetworkSocketException.h>

/**
 * Class representing a TCP socket
 * @author Andreas Drewke
 */
class miniscript::os::network::TCPSocket : public NetworkSocket {
	public:
		/**
		 * @brief Public destructor
		 */
		virtual ~TCPSocket();

		/**
		 * @brief Reads up to "bytes" bytes from socket
		 * @param buf buffer to write to
		 * @param bytes bytes to receive
		 * @throws miniscript::os::network::NetworkIOException
		 * @return bytes read
		 */
		size_t read(void* buf, const size_t bytes);

		/**
		 * @brief Writes up to "bytes" bytes to socket
		 * @param buf buffer to read from
		 * @param bytes bytes to send
		 * @throws miniscript::os::network::NetworkIOException
		 * @return bytes written
		 */
		size_t write(void* buf, const size_t bytes);

		/**
		 * @brief Creates a TCP socket
		 * @param socket socket
		 * @param ipVersion IP version
		 * @throws miniscript::os::network::NetworkSocketException
		 */
		static void create(TCPSocket& socket, IpVersion ipVersion);

		/**
		 * Connects a socket to given IP and port
		 * @param ip ip
		 * @param port port
		 * @throws miniscript::os::network::NetworkSocketException
		 */
		void connect(const string& ip, const unsigned int port);

		/**
		 * @brief Creates a TCP server socket
		 * @param socket socket
		 * @param ip ip
		 * @param port port
		 * @param backlog backlog
		 * @throws miniscript::os::network::NetworkSocketException
		 * @return socket
		 */
		static void createServerSocket(TCPSocket& socket, const std::string& ip, const unsigned int port, const int backlog);

		/**
		 * @brief Disables nagle's algorithm
		 * @throws miniscript::os::network::NetworkSocketException
		 */
		void setTCPNoDelay();

		/**
		 * @brief Accepts a socket from a server socket
		 * @param _socket socket
		 * @throws miniscript::os::network::NetworkSocketException
		 * @return if socket was accepted
		 */
		bool accept(TCPSocket& _socket);
};

