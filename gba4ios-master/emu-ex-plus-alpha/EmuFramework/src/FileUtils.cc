#include <FileUtils.hh>
#include <util/strings.h>
#include <fs/sys.hh>
#include <base/Base.hh>
#include <ftw.h>

void chdirFromFilePath(const char *path)
{
	FsSys::cPath dirnameTemp;
	FsSys::chdir(string_dirname(path, dirnameTemp));
}

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);
    
    if (rv)
        perror(fpath);
    
    return rv;
}

void fixFilePermissions(const char *path)
{
	if(FsSys::hasWriteAccess(path) == 0)
	{
		logMsg("%s lacks write permission, setting user as owner", path);
	}
	else
		return;

	FsSys::cPath execPath;
	string_printf(execPath, "%s/fixMobilePermission '%s'", Base::appPath, path);
	//logMsg("executing %s", execPath);
    
    int err = nftw(execPath, unlink_cb, 64, FTW_DEPTH | FTW_PHYS) == -1;
    if(err)
    {
        logWarn("error from fixMobilePermission helper: %d", err);
    }
    
//    int nftw(const char *path, int (*fn)(const char *,
//    const struct stat *, int, struct FTW *), int fd_limit, int flags);
    
    
//	int err = system(execPath);
//	if(err)
//	{
//		logWarn("error from fixMobilePermission helper: %d", err);
//	}
}
