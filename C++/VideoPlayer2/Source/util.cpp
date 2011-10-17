#include "util.h"

int socketId;
int playing = FALSE;

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;

int startMPlayer(const char* mPlayerPath, const char* videoPath)
{
    char arguments[1024];
	memset(&arguments, 0, sizeof(arguments));
	sprintf(arguments, " -slave -really-quiet -hardframedrop -osdlevel 0 -loop 0 %s", videoPath);

    SECURITY_ATTRIBUTES saAttr;

    // Set the bInheritHandle flag so pipe handles are inherited.
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's STDIN.
    // Create a pipe for the child process's STDIN.

    if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
    {
        fprintf(stderr, "Stdin CreatePipe\n");
        return 1;
    }

    // Ensure the write handle to the pipe for STDIN is not inherited.
    if ( ! SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) )
    {
        fprintf(stderr, "Inherit CreatePipe\n");
        return 1;
    }

	execProcess(mPlayerPath, arguments, g_hChildStd_IN_Rd, NULL, NULL);
	pauseVideo();

	return 0;
}

void execProcess(const char* szExe, const char* szArgs, HANDLE hStdIn, HANDLE hStdOut, HANDLE hStdErr)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(STARTUPINFO) );
    si.cb = sizeof(STARTUPINFO);
    si.hStdError = hStdErr;
    si.hStdOutput = hStdOut;
    si.hStdInput = hStdIn;
    si.dwFlags |= STARTF_USESTDHANDLES;

    char cmdLine[1024];
    sprintf(cmdLine, "%s%s", szExe, szArgs);
    BOOL bSuccess = FALSE;

    bSuccess = CreateProcess(NULL,
        cmdLine,        // command line
        NULL,           // process security attributes
        NULL,           // primary thread security attributes
        TRUE,           // handles are inherited
        0,              // creation flags
        NULL,           // use parent's environment
        NULL,           // use parent's current directory
        &si,            // STARTUPINFO pointer
        &pi);           // receives PROCESS_INFORMATION
}

int isPlaying()
{
    return playing;
}

int playVideo(float speed)
{
    playing = TRUE;
    return setSpeed(speed);
}

int pauseVideo()
{
    playing = FALSE;
    char message[1024];
    sprintf(message, "pausing frame_step\n");
    return sendMessage(message);
}

int setSpeed(float speed)
{
    char message[1024];
    sprintf(message, "speed_set %lf\n", speed);
    return sendMessage(message);
}

int sendMessage(const char* message)
{
    DWORD dwWritten;
    if(!WriteFile(g_hChildStd_IN_Wr, message, strlen(message), &dwWritten, NULL))
    {
        fprintf(stderr, "Could not write to pipe.\n");
        return 1;
    }
    return 0;
}

