/*
 * FTP Client - SSL/TLS (FTPS) Example
 * 
 * Demonstrates secure FTP connections using SSL/TLS
 */

#define FTP_CLIENT_IMPLEMENTATION
#include "../ftpclient.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int result;
    
    // Initialize
    if (ftp_global_init() != FTP_OK) {
        fprintf(stderr, "Failed to initialize FTP library\n");
        return 1;
    }
    
    ftp_client_t *client = ftp_client_create();
    if (!client) {
        fprintf(stderr, "Failed to create FTP client\n");
        ftp_global_cleanup();
        return 1;
    }
    
    // Configure connection for FTPS
    // Note: FTPS typically uses port 21 (explicit) or 990 (implicit)
    ftp_client_set_host(client, "ftps.example.com", 21);
    ftp_client_set_credentials(client, "username", "password");
    
    // Enable full SSL/TLS encryption
    // FTP_SSL_ALL encrypts both control and data connections
    // The second parameter (1) enables SSL certificate verification
    printf("Configuring SSL/TLS...\n");
    ftp_client_set_ssl(client, FTP_SSL_ALL, 1);
    
    // Optional: For self-signed certificates, disable verification
    // WARNING: This is less secure and vulnerable to man-in-the-middle attacks
    // ftp_client_set_ssl(client, FTP_SSL_ALL, 0);
    
    // Optional: Try SSL but fall back to plain FTP if unavailable
    // ftp_client_set_ssl(client, FTP_SSL_TRY, 1);
    
    // Optional: Encrypt only the control connection
    // ftp_client_set_ssl(client, FTP_SSL_CONTROL, 1);
    
    // Enable verbose output to see SSL handshake details
    ftp_client_set_verbose(client, 1);
    
    // Connect
    printf("Connecting securely to FTPS server...\n");
    result = ftp_client_connect(client);
    if (result != FTP_OK) {
        fprintf(stderr, "Secure connection failed: %s\n", ftp_client_get_error(client));
        ftp_client_destroy(client);
        ftp_global_cleanup();
        return 1;
    }
    printf("Secure connection established!\n\n");
    
    // List directory over secure connection
    printf("Listing directory (encrypted)...\n");
    char *listing = NULL;
    result = ftp_client_list_dir(client, "/", &listing);
    if (result == FTP_OK) {
        printf("Directory contents:\n%s\n", listing);
        free(listing);
    } else {
        fprintf(stderr, "Failed to list directory: %s\n", ftp_client_get_error(client));
    }
    
    // Upload file securely
    printf("Uploading file (encrypted)...\n");
    result = ftp_client_upload(client, "sensitive_data.txt", "/secure/sensitive_data.txt");
    if (result == FTP_OK) {
        printf("File uploaded securely!\n");
    } else {
        fprintf(stderr, "Secure upload failed: %s\n", ftp_client_get_error(client));
    }
    
    // Download file securely
    printf("Downloading file (encrypted)...\n");
    result = ftp_client_download(client, "/secure/data.txt", "secure_download.txt");
    if (result == FTP_OK) {
        printf("File downloaded securely!\n");
    } else {
        fprintf(stderr, "Secure download failed: %s\n", ftp_client_get_error(client));
    }
    
    // Cleanup
    ftp_client_destroy(client);
    ftp_global_cleanup();
    
    printf("\nSecure FTP operations completed!\n");
    return 0;
}
