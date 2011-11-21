#include "util.h"

int socketId;
int playing = FALSE;
float position;

HANDLE stdInRd = NULL;
HANDLE stdInWr = NULL;

HANDLE stdOutRd = NULL;
HANDLE stdOutWr = NULL;


int startMPlayer(const char* mPlayerPath, const char* videoPath, float initialPosition)
{
    char arguments[1024];
	memset(&arguments, 0, sizeof(arguments));
	sprintf(arguments, " -slave -hardframedrop -osdlevel 0 \"%s\"", videoPath);

    SECURITY_ATTRIBUTES saAttr;

    // Set the bInheritHandle flag so pipe handles are inherited.
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // create a pipe for the child's stdin
    if (!CreatePipe(&stdInRd, &stdInWr, &saAttr, 0) || !SetHandleInformation(stdInWr, HANDLE_FLAG_INHERIT, 0))
    {
        fprintf(stderr, "Could not create pipe for stdin.\n");
        return 1;
    }

    // create a pipe for the child's stdout
    if (!CreatePipe(&stdOutRd, &stdOutWr, &saAttr, 0) || !SetHandleInformation(stdOutRd, HANDLE_FLAG_INHERIT, 0))
    {
        fprintf(stderr, "Could not create pipe for stdout.\n");
        return 1;
    }

	execProcess(mPlayerPath, arguments, stdInRd, stdOutWr, NULL);
	seek(initialPosition);

	Sleep(2000);
	char buffer[99999];
	readMessage(buffer, true);

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

int seek(float position)
{
    char message[1024];
    sprintf(message, "pausing seek %5.2lf 2\n",  position);
    return sendMessage(message);
}

void refreshTimePos()
{
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	readMessage(buffer, false);

	char buffer2[16];
	memset(buffer2, 0, sizeof(buffer2));
	strncpy(buffer2, buffer+2, 6);

	position = atof(buffer2);
}

float getTimePos()
{
	return position;
}

int sendMessage(const char* message)
{
    DWORD dwWritten;
    if(!WriteFile(stdInWr, message, strlen(message), &dwWritten, NULL))
    {
        fprintf(stderr, "Could not write to pipe.\n");
        return 1;
    }
    return 0;
}

int readMessage(char* buffer, int wait)
{
    DWORD dwRead;
    DWORD dwAvailable;

	if(!PeekNamedPipe(stdOutRd, NULL, NULL, NULL, &dwAvailable, NULL))
	{
		fprintf(stderr, "Could not read from pipe.\n");
		return 1;
	}

	if(wait || dwAvailable > 0)
	{
		if(!ReadFile(stdOutRd, buffer, dwAvailable, &dwRead, NULL))
		{
			fprintf(stderr, "Could not read from pipe.\n");
			return 1;
		}
	}

    return 0;
}

