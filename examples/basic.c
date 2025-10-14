/*
 * FTP Client - Basic Example
 * 
 * Demonstrates basic FTP operations:
 * - Connecting to an FTP server
 * - Uploading a file
 * - Downloading a file
 * - Listing directory contents
 */

#define FTP_CLIENT_IMPLEMENTATION
#include "../ftpclient.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int result;
    
    // Initialize the FTP library
    if (ftp_global_init() != FTP_OK) {
        fprintf(stderr, "Failed to initialize FTP library\n");
        return 1;
    }
    
    // Create FTP client
    ftp_client_t *client = ftp_client_create();
    if (!client) {
        fprintf(stderr, "Failed to create FTP client\n");
        ftp_global_cleanup();
        return 1;
    }
    
    // Configure connection
    // NOTE: Replace with your FTP server details
    ftp_client_set_host(client, "ftp.example.com", 21);
    ftp_client_set_credentials(client, "username", "password");
    
    // Optional: Enable verbose output for debugging
    ftp_client_set_verbose(client, 0);
    
    // Test connection
    printf("Connecting to FTP server...\n");
    result = ftp_client_connect(client);
    if (result != FTP_OK) {
        fprintf(stderr, "Connection failed: %s\n", ftp_client_get_error(client));
        ftp_client_destroy(client);
        ftp_global_cleanup();
        return 1;
    }
    printf("Connected successfully!\n\n");
    
    // List root directory
    printf("Listing root directory...\n");
    char *listing = NULL;
    result = ftp_client_list_dir(client, "/", &listing);
    if (result == FTP_OK) {
        printf("Directory contents:\n%s\n", listing);
        free(listing);
    } else {
        fprintf(stderr, "Failed to list directory: %s\n", ftp_client_get_error(client));
    }
    
    // Upload a file
    printf("Uploading file...\n");
    result = ftp_client_upload(client, "local_file.txt", "/upload/remote_file.txt");
    if (result == FTP_OK) {
        printf("File uploaded successfully!\n");
    } else {
        fprintf(stderr, "Upload failed: %s\n", ftp_client_get_error(client));
    }
    
    // Download a file
    printf("Downloading file...\n");
    result = ftp_client_download(client, "/download/test.txt", "downloaded_file.txt");
    if (result == FTP_OK) {
        printf("File downloaded successfully!\n");
    } else {
        fprintf(stderr, "Download failed: %s\n", ftp_client_get_error(client));
    }
    
    // Get file size
    int64_t size;
    result = ftp_client_get_filesize(client, "/download/test.txt", &size);
    if (result == FTP_OK) {
        printf("File size: %lld bytes\n", (long long)size);
    }
    
    // Cleanup
    ftp_client_destroy(client);
    ftp_global_cleanup();
    
    printf("\nAll operations completed!\n");
    return 0;
}
