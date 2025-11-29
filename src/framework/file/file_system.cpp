#if CONFIG_USE_FS==1

#include "file_system.h"

void FileSystem::ListDir(const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs_.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        ListDir(file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void FileSystem::CreateDir(const char *path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs_.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void FileSystem::RemoveDir(const char *path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs_.rmdir(path)) {
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

bool FileSystem::OpenFile(const char *path, File* file) {
  Serial.printf("Reading file: %s\n", path);

  File f= fs_.open(path);
  if (!f) {
    Serial.println("Failed to open file for reading");
    return false;
  }

  file = &f;
  return true;
}

void FileSystem::WriteFile(const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs_.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
}

void FileSystem::AppendFile(const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs_.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
}

void FileSystem::RenameFile(const char *path1, const char *path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs_.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void FileSystem::DeleteFile(const char *path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs_.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

#endif //CONFIG_USE_FS