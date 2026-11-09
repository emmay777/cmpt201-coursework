#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main() {
  char *str = "hello world";
  char buf[12];

  // create "tmp" file in current folder
  // O_CREAT = create if doesn't exist
  int fd = open("tmp", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

  // write bytes to file
  ssize_t bytes_written = write(fd, str, strlen(str)); // strlen instead of sizeof(size of ptr)

  // move file offset backward by X/2 bytes
  off_t loc = lseek(fd, -6, SEEK_CUR);

  // read + print from offset -> EOF
  ssize_t bytes_read = read(fd, "", count);

  // close file
  return 0;
}
