#include "util.h"

int socketId;
int playing = FALSE;
float position;

HANDLE stdInRd = NULL;
HANDLE stdInWr = NULL;

HANDLE stdOutRd = NULL;
HANDLE stdOutWr = NULL;

int socketIdIn;
struct sockaddr_in addressIn;

int socketIdOut;
struct sockaddr_in addressOus;

char bufIn[BUFFER_LENGTH];
char bufOut[BUFFER_LENGTH];

int steerValue = 0;
int throttleValue = 0;

int setupSockets(int udpPortIn, int udpPortOut, const char* remoteAddress)
{

	#ifdef OS_WINDOWS
		WSADATA wsa;
		if(WSAStartup(MAKEWORD(2, 0), &wsa) != 0)
		{
			fprintf(stderr, "Could not start WinSock.");
			return 1;
		}
	#endif

	// create in socket
	socketIdIn = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketIdIn < 0)
	{
		fprintf(stderr, "Could not open in socket.\n");
		return 1;
	}
	memset(&addressIn, 0, sizeof(addressIn));
	addressIn.sin_family = AF_INET;
	addressIn.sin_addr.s_addr = INADDR_ANY;
	addressIn.sin_port = htons(udpPortIn);
	if(bind(socketIdIn, (sockaddr *)&addressIn, sizeof(addressIn)) < 0)
	{
		fprintf(stderr, "Could not bind to in socket.\n");
		return 1;
	}
	printf("Reading from *:%d\n", udpPortIn);

	// create out socket
	socketIdOut = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketIdOut < 0)
	{
		fprintf(stderr, "Could not open out socket.\n");
		return 1;
	}
	memset(&addressOus, 0, sizeof(addressOus));
	addressOus.sin_family = AF_INET;
	addressOus.sin_addr.s_addr = inet_addr(remoteAddress);
	addressOus.sin_port = htons(udpPortOut);
	if(connect(socketIdOut, (sockaddr *)&addressOus, sizeof(addressOus)) < 0)
	{
		fprintf(stderr, "Could not connect to out socket.\n");
		return 1;
	}
	printf("Writing to %s:%d\n", remoteAddress, udpPortOut);

	return 0;
}

int readFromSocket()
{
	// read message
	memset(&bufIn, 0, BUFFER_LENGTH);
	int msgLength = (int)recv(socketIdIn, bufIn, BUFFER_LENGTH, 0);
	if (msgLength < 0)
	{
		fprintf(stderr, "Error in function recv.\n");
		return 1;
	}

	char* str = "0";
	char* thr = "0";
	char* p = strtok(bufIn, ",");
	if(p != NULL)
	{
		str = p;
		p = strtok(NULL, ",");
		if(p != NULL)
		{
			thr = p;
		}
	}

	steerValue = atoi(str);
	throttleValue = atoi(thr);
}

int writeToSocket(float speed, float videoTimePos)
{
	// send message
	memset(&bufOut, 0, BUFFER_LENGTH);
	sprintf(bufOut, "%.2f%c%.1f\n", speed, SEPARATING_CHARACTER, videoTimePos);
	//sprintf(bufOut, "%.2f\n", speed);

	int msgLength = (int)sendto(socketIdOut, bufOut, BUFFER_LENGTH, 0, (sockaddr *)&addressOus, sizeof(addressOus));
	if (msgLength < 0)
	{
		fprintf(stderr, "Error in function sendto.\n");
		return 1;
	}
}

int getSteerValue()
{
	return steerValue;
}

int getThrottleValue()
{
	return throttleValue;
}

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

	bSuccess = CreateProcess(NULL, cmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
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

	float newPosition = atof(buffer2);

	if(newPosition > 0)
		position = newPosition;
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

