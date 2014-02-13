/* vim: set ts=4 sw=4 tw=0 et ai :*/

#include "src/package-fs/logging.h"
#include "src/package-fs/internal/fuselink.h"
#include "config.h"
#include "funcdefs.h"

std::string global_mount_path = "<not set>";

int appmount_start(int argc, char *argv[])
{
    const char *disk_path = NULL;
    const char *mount_path = NULL;

    if (argc < 3)
    {
        std::cerr << "packagemount <diskimage> <mountpoint>" << std::endl;
        return 1;
    }

    // Set the application name.
    AppLib::Logging::setApplicationName(std::string("appmount"));

    // Store the disk path and mount point the user has provided.
    disk_path = argv[1];
    mount_path = argv[2];
    global_mount_path = mount_path;

    // Open the file for our lock checks / sets.
    /*int lockedfd = open(disk_image->filename[0], O_RDWR);
     * bool locksuccess = true;
     * flock lock = { F_RDLCK, SEEK_SET, 0, 0, 0 };
     *
     * // Check to see whether there is a lock on the file already.
     * int lockres = fcntl(lockedfd, F_GETLK, &lock);
     * if (lockres == -1)
     * locksuccess = false;
     * if (locksuccess)
     * if (lock.l_type != F_UNLCK)
     * locksuccess = false;
     * if (!locksuccess)
     * {
     * AppLib::Logging::showErrorW("Unable to lock image.  Check to make sure it's");
     * AppLib::Logging::showErrorO("not already mounted.");
     * return 1;
     * } */

    // Lock the specified disk file.
    /*lock = { F_WRLCK, SEEK_SET, 0, 0, 0 };
     * lockres = fcntl(lockedfd, F_SETLK, &lock);
     * int ret = 1;
     * if (lockres != -1)
     * { */
    AppLib::Logging::showInfoW("The application package will now be mounted at:");
    AppLib::Logging::showInfoO("  * %s", global_mount_path.c_str());
    AppLib::Logging::showInfoO("You can use fusermount (or umount if root) to unmount the");
    AppLib::Logging::showInfoO("application package.  Please note that the package is locked");
    AppLib::Logging::showInfoO("while mounted and that no other operations can be performed");
    AppLib::Logging::showInfoO("on it while this is the case.");

    AppLib::FUSE::Mounter * mnt = new AppLib::FUSE::Mounter(disk_path, mount_path, true, false, appmount_continue);
    int ret = mnt->getResult();

    if (ret != 0)
    {
        // Mount failed.
        AppLib::Logging::showErrorW("FUSE was unable to mount the application package.");
        AppLib::Logging::showErrorO("Check that the package is a valid AppFS filesystem and");
        AppLib::Logging::showErrorO("run 'apputil check' to scan for filesystem errors.");
    }
    else
    {
        // Unlock the file.
        // TODO: Find out documentation on unlocking files.
    }

    return ret;
    /*}
     * else
     * {
     * AppLib::Logging::showErrorW("Unable to lock image.  Check to make sure it's");
     * AppLib::Logging::showErrorO("not already mounted.");
     * return 1;
     * } */

    return 0;
}

void appmount_continue()
{
    // Execution continues at this point when the filesystem is mounted.
}

int main(int argc, char *argv[])
{
    return appmount_start(argc, argv);
}
