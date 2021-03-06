// -*- mode: cpp; mode: fold -*-
// Description								/*{{{*/
/* ######################################################################

   Index Copying - Aid for copying and verifying the index files
   
   ##################################################################### */
									/*}}}*/
#ifndef INDEXCOPY_H
#define INDEXCOPY_H

#include <vector>
#include <string>
#include <stdio.h>

#include <apt-pkg/macros.h>

#ifndef APT_10_CLEANER_HEADERS
#include <apt-pkg/gpgv.h>
class FileFd;
#endif
#ifndef APT_8_CLEANER_HEADERS
using std::string;
using std::vector;
#endif

class pkgTagSection;
class indexRecords;
class pkgCdromStatus;
class FileFd;

class IndexCopy								/*{{{*/
{
   /** \brief dpointer placeholder (for later in case we need it) */
   void *d;

   protected:
   
   pkgTagSection *Section;
   
   std::string ChopDirs(std::string Path,unsigned int Depth);
   bool ReconstructPrefix(std::string &Prefix,std::string OrigPath,std::string CD,
			  std::string File);
   bool ReconstructChop(unsigned long &Chop,std::string Dir,std::string File);
   void ConvertToSourceList(std::string CD,std::string &Path);
   bool GrabFirst(std::string Path,std::string &To,unsigned int Depth);
   virtual bool GetFile(std::string &Filename,unsigned long long &Size) = 0;
   virtual bool RewriteEntry(FileFd &Target, std::string const &File) = 0;
   virtual const char *GetFileName() = 0;
   virtual const char *Type() = 0;
   
   public:

   bool CopyPackages(std::string CDROM,std::string Name,std::vector<std::string> &List,
		     pkgCdromStatus *log);
   IndexCopy();
   virtual ~IndexCopy();
};
									/*}}}*/
class PackageCopy : public IndexCopy					/*{{{*/
{
   void *d;
   protected:

   virtual bool GetFile(std::string &Filename,unsigned long long &Size);
   virtual bool RewriteEntry(FileFd &Target, std::string const &File);
   virtual const char *GetFileName() {return "Packages";};
   virtual const char *Type() {return "Package";};

   public:
   PackageCopy();
   virtual ~PackageCopy();
};
									/*}}}*/
class SourceCopy : public IndexCopy					/*{{{*/
{
   void *d;
   protected:
   
   virtual bool GetFile(std::string &Filename,unsigned long long &Size);
   virtual bool RewriteEntry(FileFd &Target, std::string const &File);
   virtual const char *GetFileName() {return "Sources";};
   virtual const char *Type() {return "Source";};

   public:
   SourceCopy();
   virtual ~SourceCopy();
};
									/*}}}*/
class TranslationsCopy							/*{{{*/
{
   void *d;
   protected:
   pkgTagSection *Section;

   public:
   bool CopyTranslations(std::string CDROM,std::string Name,std::vector<std::string> &List,
			 pkgCdromStatus *log);

   TranslationsCopy();
   virtual ~TranslationsCopy();
};
									/*}}}*/
class SigVerify								/*{{{*/
{
   /** \brief dpointer placeholder (for later in case we need it) */
   void *d;

   APT_HIDDEN bool Verify(std::string prefix,std::string file, indexRecords *records);
   APT_HIDDEN bool CopyMetaIndex(std::string CDROM, std::string CDName,
		      std::string prefix, std::string file);

 public:
   bool CopyAndVerify(std::string CDROM,std::string Name,std::vector<std::string> &SigList,
		      std::vector<std::string> PkgList,std::vector<std::string> SrcList);

   APT_DEPRECATED static bool RunGPGV(std::string const &File, std::string const &FileOut,
		       int const &statusfd, int fd[2]);
   APT_DEPRECATED static bool RunGPGV(std::string const &File, std::string const &FileOut,
			      int const &statusfd = -1);

   SigVerify();
   virtual ~SigVerify();
};
									/*}}}*/

#endif
