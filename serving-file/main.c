#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
	FILE *html = fopen("index.html", "r");

	fseek(html, 0, SEEK_END);
	unsigned long long html_size = ftell(html);
	fseek(html, 0, SEEK_SET);

	char *html_body = malloc(html_size);
	fread(html_body, sizeof(char), html_size, html);
	fclose(html);

	char *http_header =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"\r\n";

	struct sockaddr_in socket_address, client_address;
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(8080);
	socket_address.sin_addr.s_addr = INADDR_ANY;
	socklen_t client_address_len = sizeof(client_address);

	unsigned short socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	bind(socket_fd, (struct sockaddr *) &socket_address, sizeof(socket_address));
	listen(socket_fd, SOMAXCONN);

	while (1)
	{
		unsigned short client_fd = accept(socket_fd, (struct sockaddr *) &client_address, &client_address_len);

		send(client_fd, http_header, strlen(http_header), 0);
		send(client_fd, html_body, html_size, 0);
		close(client_fd);
	}

	close(socket_fd);
	return 0;
}
