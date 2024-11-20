/* ESP8266 Librería LittleFS
 *
 * ESP8266 LOLIN(WEMOS) D1 mini (clone).
 *
 * Librería LittleFS.
 * 
 * Uso y gestión de la memoria Flash:
 * - Creación de archivos
 * - Lectura
 * - Modificación
 * - Borrado
 * 
 * - Ver archivos
 * - Formatear archivos
 *
 * Autor: Carlos Muñoz
 * Información: https://www.infotronikblog.com/2024/11/esp8266-como-usar-littlefs-para-guardar.html
 * Web: https://www.infotronikblog.com
 */

#include "LittleFS.h"

String data = "Some data to store on a file";

void setup() {
  Serial.begin(115200);
  delay(100);
  // put your setup code here, to run once:
  if (!LittleFS.begin()){
    Serial.println("Error montando LittleFS");
    return;
  } 

  Serial.println("\n\nLittleFS se cargo correctamente");
  Create_file("/data.txt", data);
  Delete_file("/data.txt");
  // Read_file("/data.txt");
  // Add_data_file("/data.txt", data);
  // Read_file("/data.txt");
  // Serial.println();
  Show_filetree("/");
  // Show_memory();
  // Format_all();
}


void Create_file(const char *path, String data)
{
  Serial.println(path);
  File file = LittleFS.open(path, "w");
  if (!file){
    Serial.println("Error open file");
    return;
  }

  if (file.print(data)){
    Serial.println("Writing data");
  } else {
    Serial.println("Error writing data");
  }
  file.close();
}

void Add_data_file(const char *path, String data)
{
  Serial.println(path);
  File file = LittleFS.open(path, "a");
  if (!file){
    Serial.println("Error open file");
    return;
  }

  if (file.print(data)){
    Serial.println("Adding data");
  } else {
    Serial.println("Error writing file");
  }
  file.close();
}

void Read_file(const char *path)
{
   File file = LittleFS.open(path, "r");
  if (!file){
    Serial.println("Error reading file");
    return;
  }

  Serial.println("File content: ");
  while (file.available())
  {
    Serial.write(file.read());
  }
  Serial.println();
  file.close();
}


void Delete_file(const char *filename)
{
  if (LittleFS.remove(filename)){
    Serial.print("Removed ");
    Serial.println(filename);
  } else {
    Serial.println("Deleting error");
  }
}

void Format_all()
{
  if (!LittleFS.format()){
    Serial.println("Error montando LittleFS");
    return;
  } else {
    Serial.println("All files deleted");
  }
  LittleFS.end();
}

void Show_filetree(const char *path)
{
  Dir folder = LittleFS.openDir(path);
  while (folder.next())
  {
    File file = folder.openFile("r");

  Serial.print(" Name: ");
  Serial.print(file.name());
  Serial.print(" Size: ");
  Serial.print(file.size());
  Serial.print(" Folder:");
  Serial.print(file.isDirectory() ? "Yes" : "No");
  Serial.println();
  file.close();
  }
    Serial.println("End file off filetree");
}

void Show_memory()
{
  FSInfo fs_info;
  LittleFS.info(fs_info);

  Serial.print("Used: ");
  int used = fs_info.usedBytes / 1024;
  Serial.print(used);
 
  Serial.print(" total Kbit: ");
  int total_kbits = (fs_info.totalBytes / 1024);
  Serial.print(total_kbits);
  Serial.print(" Used Kbits Used Memory: " );
  int used_memory = (fs_info.usedBytes / fs_info.totalBytes) * 100;
  Serial.print(used_memory);
  Serial.println();
  
}

char* ConvertStringToCharArray(String S)
{
  int ArrayLength = S.length() + 1;
  char CharArray[ArrayLength];
  S.toCharArray(CharArray,ArrayLength);
  return(CharArray);
}

void loop() {
  // put your main code here, to run repeatedly:

}
