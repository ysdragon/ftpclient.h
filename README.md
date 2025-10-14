# ftpclient.h - Single Header Library

A simple, lightweight FTP client library for C/C++ built on top of libcurl. This is a stb-style single-header library - just drop it into your project and you're ready to go.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://ysdragon.github.io/ftpclient.h/)

## Features

- 🚀 **Single header file** - Just include and use
- 📦 **Zero dependencies** (except libcurl)
- 🔒 **SSL/TLS support** - Secure FTPS connections
- 📤 **Upload & Download** - File transfer operations
- 📁 **Directory operations** - Create, remove, list directories
- 🔄 **Transfer progress callbacks** - Track upload/download progress
- ⚙️ **Passive/Active mode** - Flexible transfer modes
- 🛠️ **Custom commands** - Execute raw FTP commands
- 🔧 **Configurable timeouts** - Connection and transfer timeouts

## Quick Start

### Installation

Just copy `ftpclient.h` into your project. That's it!

### Basic Usage

```c
#define FTP_CLIENT_IMPLEMENTATION
#include "ftpclient.h"

int main(void) {
    // Initialize library
    ftp_global_init();
    
    // Create client
    ftp_client_t *client = ftp_client_create();
    
    // Configure connection
    ftp_client_set_host(client, "ftp.example.com", 21);
    ftp_client_set_credentials(client, "username", "password");
    
    // Test connection
    if (ftp_client_connect(client) == FTP_OK) {
        printf("Connected successfully!\n");
        
        // Upload a file
        ftp_client_upload(client, "local_file.txt", "/remote/file.txt");
        
        // Download a file
        ftp_client_download(client, "/remote/data.txt", "downloaded.txt");
        
        // List directory
        char *listing = NULL;
        if (ftp_client_list_dir(client, "/", &listing) == FTP_OK) {
            printf("Directory contents:\n%s\n", listing);
            free(listing);
        }
    } else {
        fprintf(stderr, "Connection failed: %s\n", ftp_client_get_error(client));
    }
    
    // Cleanup
    ftp_client_destroy(client);
    ftp_global_cleanup();
    
    return 0;
}
```

### Compilation

#### Option 1: Direct Compilation

You need to have libcurl installed and link against it:

**Linux/macOS**
```bash
gcc -o myprogram myprogram.c -lcurl
```

**Windows (MinGW)**
```bash
gcc -o myprogram.exe myprogram.c -lcurl
```

**Windows (MSVC)**
```bash
cl /Fe:myprogram.exe myprogram.c libcurl.lib
```

#### Option 2: Using CMake

The library includes CMake support for easy integration:

**Basic usage:**
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

**Options:**
```bash
# Build with examples
cmake .. -DBUILD_EXAMPLES=ON

# Install the library
cmake --build . --target install
```

**Using in your CMake project:**

Add to your `CMakeLists.txt`:
```cmake
find_package(ftpclient REQUIRED)
target_link_libraries(your_target PRIVATE ftpclient::ftpclient)
```

Or use it as a subdirectory:
```cmake
add_subdirectory(path/to/ftpclient)
target_link_libraries(your_target PRIVATE ftpclient)
```

## Documentation

📚 **[Full API Documentation](https://ysdragon.github.io/ftpclient.h/)** - Complete API reference with detailed descriptions, examples, and usage guides.

### Initialization

```c
// Initialize library (call once at program start)
int ftp_global_init(void);

// Cleanup library (call once at program end)
void ftp_global_cleanup(void);

// Create a new FTP client handle
ftp_client_t *ftp_client_create(void);

// Destroy client handle
void ftp_client_destroy(ftp_client_t *client);
```

### Configuration

```c
// Set server host and port
ftp_client_set_host(client, "ftp.example.com", 21);

// Set authentication credentials
ftp_client_set_credentials(client, "username", "password");

// Set transfer mode (FTP_MODE_PASSIVE or FTP_MODE_ACTIVE)
ftp_client_set_mode(client, FTP_MODE_PASSIVE);

// Configure SSL/TLS
// Modes: FTP_SSL_NONE, FTP_SSL_TRY, FTP_SSL_CONTROL, FTP_SSL_ALL
ftp_client_set_ssl(client, FTP_SSL_ALL, 1);  // 1 = verify certificates

// Set timeouts (in seconds)
ftp_client_set_timeout(client, 120, 30);  // operation timeout, connect timeout

// Enable verbose debug output
ftp_client_set_verbose(client, 1);
```

### File Operations

```c
// Upload a file
int ftp_client_upload(ftp_client_t *client, 
                      const char *local_path, 
                      const char *remote_path);

// Download a file
int ftp_client_download(ftp_client_t *client, 
                        const char *remote_path, 
                        const char *local_path);

// Get file size
int64_t size;
ftp_client_get_filesize(client, "/remote/file.txt", &size);

// Delete a file
ftp_client_delete(client, "/remote/file.txt");

// Rename or move a file
ftp_client_rename(client, "/old/path.txt", "/new/path.txt");
```

### Directory Operations

```c
// List directory contents
char *listing = NULL;
if (ftp_client_list_dir(client, "/path", &listing) == FTP_OK) {
    printf("%s\n", listing);
    free(listing);
}

// Create directory
ftp_client_mkdir(client, "/new_folder");

// Remove empty directory
ftp_client_rmdir(client, "/old_folder");
```

### Progress Tracking

```c
// Progress callback function
int my_progress(void *user_data, double dltotal, double dlnow, 
                double ultotal, double ulnow) {
    if (ultotal > 0) {
        double percent = (ulnow / ultotal) * 100.0;
        printf("Upload: %.1f%% (%.0f / %.0f bytes)\n", 
               percent, ulnow, ultotal);
    }
    return 0;  // Return non-zero to abort transfer
}

// Set progress callback
ftp_client_set_progress_callback(client, my_progress, NULL);
```

### Error Handling

```c
int result = ftp_client_upload(client, "file.txt", "/remote.txt");
if (result != FTP_OK) {
    fprintf(stderr, "Error: %s\n", ftp_client_get_error(client));
}
```

### Error Codes

| Code | Value | Description |
|------|-------|-------------|
| `FTP_OK` | 0 | Success |
| `FTP_ERROR_INIT` | -1 | Library initialization failed |
| `FTP_ERROR_CONNECTION` | -2 | Connection failed |
| `FTP_ERROR_AUTH` | -3 | Authentication failed |
| `FTP_ERROR_TRANSFER` | -4 | Transfer operation failed |
| `FTP_ERROR_FILE_NOT_FOUND` | -5 | Remote file not found |
| `FTP_ERROR_MEMORY` | -6 | Memory allocation failed |
| `FTP_ERROR_INVALID_PARAM` | -7 | Invalid parameter |
| `FTP_ERROR_CURL` | -8 | libcurl error |
| `FTP_ERROR_FILE_IO` | -9 | Local file I/O error |
| `FTP_ERROR_TIMEOUT` | -10 | Operation timed out |

## Advanced Usage

### FTPS (FTP over SSL/TLS)

```c
// Enable full SSL/TLS encryption with certificate verification
ftp_client_set_ssl(client, FTP_SSL_ALL, 1);

// For self-signed certificates, you may need to disable verification
// (WARNING: This makes connections vulnerable to MITM attacks)
ftp_client_set_ssl(client, FTP_SSL_ALL, 0);
```

### Custom FTP Commands

```c
char *response = NULL;
if (ftp_client_execute_command(client, "STAT", &response) == FTP_OK) {
    printf("Server status:\n%s\n", response);
    free(response);
}
```

### Configuration Macros

You can customize buffer sizes by defining these macros before including the header:

```c
#define FTP_MAX_URL_LENGTH 4096    // Default: 2048
#define FTP_BUFFER_SIZE 16384      // Default: 8192
#define FTP_CLIENT_IMPLEMENTATION
#include "ftpclient.h"
```

## Examples

See the [examples](examples/) directory for complete working examples.

## Requirements

- C compiler (C99 or later)
- libcurl (7.20.0 or later)
  - On Ubuntu/Debian: `sudo apt-get install libcurl4-openssl-dev`
  - On Fedora/RHEL: `sudo dnf install libcurl-devel`
  - On macOS: `brew install curl`
  - On FreeBSD: `pkg install curl`
  - On Windows: Download from [curl.se](https://curl.se/download.html)

## Platform Support

- ✅ Linux
- ✅ macOS
- ✅ Windows (MinGW, MSVC)
- ✅ BSD variants
- ✅ Any platform with libcurl support

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## License

This library is released under the MIT License. See [LICENSE](LICENSE) for details.