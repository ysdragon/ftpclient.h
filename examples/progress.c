/*
 * FTP Client - Progress Callback Example
 * 
 * Demonstrates how to track upload/download progress
 */

#define FTP_CLIENT_IMPLEMENTATION
#include "../ftpclient.h"
#include <stdio.h>

// Progress callback function
int progress_callback(void *user_data, double dltotal, double dlnow, 
                     double ultotal, double ulnow)
{
    // Display upload progress
    if (ultotal > 0) {
        double percent = (ulnow / ultotal) * 100.0;
        printf("\rUpload Progress: %6.2f%% (%.0f / %.0f bytes)", 
               percent, ulnow, ultotal);
        fflush(stdout);
    }
    
    // Display download progress
    if (dltotal > 0) {
        double percent = (dlnow / dltotal) * 100.0;
        printf("\rDownload Progress: %6.2f%% (%.0f / %.0f bytes)", 
               percent, dlnow, dltotal);
        fflush(stdout);
    }
    
    // Return 0 to continue, non-zero to abort
    return 0;
}

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
    
    // Set progress callback
    ftp_client_set_progress_callback(client, progress_callback, NULL);
    
    // Connect
    printf("Connecting...\n");
    result = ftp_client_connect(client);
    if (result != FTP_OK) {
        fprintf(stderr, "Connection failed: %s\n", ftp_client_get_error(client));
        ftp_client_destroy(client);
        ftp_global_cleanup();
        return 1;
    }
    
    // Upload with progress
    printf("Starting upload...\n");
    result = ftp_client_upload(client, "large_file.bin", "/upload/large_file.bin");
    printf("\n");  // New line after progress
    
    if (result == FTP_OK) {
        printf("Upload completed successfully!\n");
    } else {
        fprintf(stderr, "Upload failed: %s\n", ftp_client_get_error(client));
    }
    
    // Download with progress
    printf("Starting download...\n");
    result = ftp_client_download(client, "/download/large_file.bin", "downloaded_large.bin");
    printf("\n");  // New line after progress
    
    if (result == FTP_OK) {
        printf("Download completed successfully!\n");
    } else {
        fprintf(stderr, "Download failed: %s\n", ftp_client_get_error(client));
    }
    
    // Cleanup
    ftp_client_destroy(client);
    ftp_global_cleanup();
    
    return 0;
}
