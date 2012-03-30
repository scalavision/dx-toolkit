#ifndef DXCPP_BINDINGS_DXFILE_H
#define DXCPP_BINDINGS_DXFILE_H

#include "../bindings.h"

class DXFile: public DXClass {
 private:
  int pos_; /* For use when reading closed remote files; stores the
	     * current position (in bytes from the beginning of the
	     * file) from which future read() calls will begin.
	     */
  int file_length_; /* For use when reading closed remote files;
		     *  stores length of the file so that accurate
		     *  byte ranges can be requested.
		     */
  std::string buffer_; /* For use when writing remote files; stores a
		   * buffer of data that will be periodically flushed
		   * to the API server.
		   */
  int cur_part_; /* For use when writing remote files; stores the
		  * part index to be used on the next part to be
		  * uploaded to the API server.
		  */
  bool eof_; /* Indicates when end of file has been reached when
	      * reading a remote file.
	      */

 public:
  /** Describes the object.
   * @see DXClass::describe()
   */
  dx::JSON describe() const { return fileDescribe(dxid_); }
  dx::JSON getProperties(const dx::JSON &keys) const { return fileGetProperties(dxid_, keys); }
  void setProperties(const dx::JSON &properties) const { fileSetProperties(dxid_, properties); }
  void addTypes(const dx::JSON &types) const { fileAddTypes(dxid_, types); }
  void removeTypes(const dx::JSON &types) const { fileRemoveTypes(dxid_, types); }
  void destroy() { fileDestroy(dxid_); }

  // File-specific functions

  void setID(const std::string &dxid);
  void create();
  void read(char* s, int n);
  bool eof() const;
  void seek(const int pos);
  void flush();
  void write(const char* s, int n);
  void uploadPart();
  bool is_open() const;
  void close(const bool block=false) const;
  void waitOnClose() const;

  // TODO: Provide streaming operators for all reasonable types
  DXFile & operator<<(bool foo);
  DXFile & operator>>(bool foo);

  static DXFile openDXFile(const std::string &dxid);

  static DXFile newDXFile(const std::string &mediaType="");

  static void downloadDXFile(const std::string &dxid, const std::string &filename, int chunksize=1048576);

  static DXFile uploadLocalFile(const std::string &filename, const std::string &media_type="");

  // Do we even want uploadString?  What about uploadCharBuffer?
  static DXFile uploadString(const std::string &to_upload, const std::string &media_type="");
};

#endif
