/*
 * FTP Client - Directory Operations Example
 * 
 * Demonstrates directory management operations:
 * - Creating directories
 * - Listing directory contents
 * - Renaming/moving files
 * - Deleting files and directories
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
    
    // Configure connection
    ftp_client_set_host(client, "ftp.example.com", 21);
    ftp_client_set_credentials(client, "username", "password");
    
    // Connect
    printf("Connecting to FTP server...\n");
    result = ftp_client_connect(client);
    if (result != FTP_OK) {
        fprintf(stderr, "Connection failed: %s\n", ftp_client_get_error(client));
        ftp_client_destroy(client);
        ftp_global_cleanup();
        return 1;
    }
    printf("Connected!\n\n");
    
    // Create a new directory
    printf("Creating directory /test_folder...\n");
    result = ftp_client_mkdir(client, "/test_folder");
    if (result == FTP_OK) {
        printf("Directory created successfully!\n");
    } else {
        fprintf(stderr, "Failed to create directory: %s\n", ftp_client_get_error(client));
    }
    
    // List root directory
    printf("\nListing root directory...\n");
    char *listing = NULL;
    result = ftp_client_list_dir(client, "/", &listing);
    if (result == FTP_OK) {
        printf("Contents:\n%s\n", listing);
        free(listing);
    }
    
    // Upload a test file to the new directory
    printf("Uploading test file...\n");
    result = ftp_client_upload(client, "test.txt", "/test_folder/test.txt");
    if (result == FTP_OK) {
        printf("File uploaded!\n");
    } else {
        fprintf(stderr, "Upload failed: %s\n", ftp_client_get_error(client));
    }
    
    // List the new directory
    printf("\nListing /test_folder...\n");
    result = ftp_client_list_dir(client, "/test_folder", &listing);
    if (result == FTP_OK) {
        printf("Contents:\n%s\n", listing);
        free(listing);
    }
    
    // Rename the file
    printf("Renaming file...\n");
    result = ftp_client_rename(client, "/test_folder/test.txt", "/test_folder/renamed.txt");
    if (result == FTP_OK) {
        printf("File renamed successfully!\n");
    } else {
        fprintf(stderr, "Rename failed: %s\n", ftp_client_get_error(client));
    }
    
    // Move file to another directory
    printf("Moving file to root...\n");
    result = ftp_client_rename(client, "/test_folder/renamed.txt", "/moved_file.txt");
    if (result == FTP_OK) {
        printf("File moved successfully!\n");
    } else {
        fprintf(stderr, "Move failed: %s\n", ftp_client_get_error(client));
    }
    
    // Get file size
    int64_t size;
    result = ftp_client_get_filesize(client, "/moved_file.txt", &size);
    if (result == FTP_OK) {
        printf("File size: %lld bytes\n", (long long)size);
    }
    
    // Delete the moved file
    printf("Deleting file...\n");
    result = ftp_client_delete(client, "/moved_file.txt");
    if (result == FTP_OK) {
        printf("File deleted successfully!\n");
    } else {
        fprintf(stderr, "Delete failed: %s\n", ftp_client_get_error(client));
    }
    
    // Remove the empty directory
    printf("Removing directory...\n");
    result = ftp_client_rmdir(client, "/test_folder");
    if (result == FTP_OK) {
        printf("Directory removed successfully!\n");
    } else {
        fprintf(stderr, "Remove directory failed: %s\n", ftp_client_get_error(client));
    }
    
    // Execute custom FTP command
    printf("\nExecuting STAT command...\n");
    char *response = NULL;
    result = ftp_client_execute_command(client, "STAT", &response);
    if (result == FTP_OK && response) {
        printf("Server status:\n%s\n", response);
        free(response);
    }
    
    // Cleanup
    ftp_client_destroy(client);
    ftp_global_cleanup();
    
    printf("\nAll directory operations completed!\n");
    return 0;
}
