#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include <curl/curl.h>
//#include <json-c/json.h>
#include <time.h>
#include <jansson.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>

#define API_KEY "a19904cb740adbce4e64a726724e34c5"
#define CITY_NAME "Quang Ngai"
#define INPUT_SIZE 500
#define INPUT_SIZE2 10
#define CUTTING_WORD " \n"
#define ENDING_WORD "exit"
#define PORT 12345
// Màu cho text
#define RED_TEXT "\x1B[31m"
#define GREEN_TEXT "\x1B[32m"
#define YELLOW_TEXT "\x1B[33m"
#define WHITE_TEXT "\x1B[37m"
#define BG_BLUE "\x1B[44m"
#define BG_WHITE "\x1B[47m"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define FILE_NAME "tasks.txt"

void DisplayPrompt();
char** execFunction(char *input, char **argv, int *sizeOfArray, int *cmdLength);
void garbageCollector(char **argv, int size);
void init_shell();
void helpcommand();
void handleMode();
void subFunction1();
void subFunction2();
void subFunction3();
void subFunction4();
void handleIpInfo();
void playMusic();
void get_weather_info();
void chatcommand();
void subFunctionChat1();
void subFunctionChat2();
void loadTasksFromFile();
void saveTasksToFile();
void manageTasks();
void removeTask();
void addTask();
void displayTasks();
void* connectThread(void* arg);
void *handleClient(void *arg);
void displayWeatherInfo(const char *jsonString);
void displayCalendar(int year, int month);
void handleday();
void handleDay2();
void handlecamera();
void handlegame();
void generateSecretNumber();
void* guessThread(void* arg);
void git_info();
void get_github_status(const char *username);
void display_system_info();
void display_cpu_info();
void display_disk_info();
void parseRss(const char *url);
void send_email();
void media();
void run_cowsay(const char *input);

static int *numOfCmd;
static int *cmdLength;

#define MAX_TASKS 100

typedef struct {
    char name[50];
    char description[100];
    char deadline[20];
} Task;

Task tasks[MAX_TASKS];
int numTasks = 0;

//game
#define MAX_GUESS 100
int exitGame = 0; // New variable to control game exit
int secretNumber;
int guess;
int attempts = 0;

// Pipe descriptors
int pipefd[2];


int main() {
    numOfCmd = mmap(NULL, sizeof *numOfCmd, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    cmdLength = mmap(NULL, sizeof *cmdLength, PROT_READ | PROT_WRITE,
                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    (*numOfCmd) = 0;
    (*cmdLength) = 0;
    system("sl");
    init_shell();

    int sizeOfArray = 0;
    char input[INPUT_SIZE] = "";

    system("linuxlogo");
    system("espeak 'hello welcome to my shell'");

    system("echo \"Please enter the one you want or enter 'help' for help\" | pv -qL 10");
    DisplayPrompt();

    pid_t id;
    char **argv;
    setbuf(stdout, NULL);  // Disable buffering
    
    while (strcmp(input, ENDING_WORD) != 0) {
        if (fgets(input, INPUT_SIZE, stdin) == NULL)
            printf(" ");

        argv = execFunction(input, argv, &sizeOfArray, cmdLength);
        
        if (strcmp("cd", argv[0]) == 0) {
            struct passwd *pwd;
            char *path = argv[1];

            if (path == NULL) {
                pwd = getpwuid(getuid());
                path = pwd->pw_dir;
            }

            if (path[0] == '/')
                path++;

            errno = chdir(path);

            if (errno != 0)
                printf("error changing directory\n");

            DisplayPrompt();
        } else if (strcmp("help", argv[0]) == 0) {
            helpcommand(); // help for command in my shell
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else if (strcmp("chat", argv[0]) == 0) {
            chatcommand(); // help for command in my shell
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else if (strcmp("mode", argv[0]) == 0) {
            handleMode(); // Enter mode and display options
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else if (strcmp("ipinfo", argv[0]) == 0) {
            handleIpInfo();
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else if (strcmp("time", argv[0]) == 0) {
            handleday();
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else if (strcmp("day", argv[0]) == 0) {
            handleDay2();
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else if (strcmp("camera", argv[0]) == 0) {
            handlecamera();
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else if (strcmp("game", argv[0]) == 0) {
            handlegame();
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else if (strcmp("git", argv[0]) == 0) {
            git_info();
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else if (strcmp("media", argv[0]) == 0) {
            media();
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else if (strcmp("say", argv[0]) == 0) {
            char input[256];
	    // Nhập chuỗi từ người dùng
	    printf("type your lyric: ");
	    fgets(input, sizeof(input), stdin);
	    // Loại bỏ ký tự new line từ chuỗi nhập
	    input[strcspn(input, "\n")] = '\0';
	    // Chạy lệnh cowsay với chuỗi nhập
	    run_cowsay(input);
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else if (strcmp("github", argv[0]) == 0) {
            const char *username = "nlhtien"; // Thay đổi tên người dùng tại đây
    	    get_github_status(username);
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else if (strcmp("system", argv[0]) == 0) {
            // Hiển thị thông tin hệ thống
	    display_system_info();
	    // Hiển thị thông tin CPU
	    display_cpu_info();
	    // Hiển thị thông tin ổ đĩa
	    display_disk_info();
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else if (strcmp("rss", argv[0]) == 0) {
            parseRss("https://news.google.com/rss?hl=en-US&gl=US&ceid=US:en");
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else if (strcmp("gmail", argv[0]) == 0) {
            const char *to = "nlhtien2002@gmail.com";
            const char *subject = "Test Email";
            const char *body = "Hello,\n\nThis is a test email sent from a C program using libcurl.\n\nRegards,\nYour Name";

    	    send_email(to, subject, body);
            if (strcmp(input, ENDING_WORD) != 0) {
                DisplayPrompt();
            }
        } else {
            id = fork();
            if (id < 0) {
                printf("fork failed");
                exit(0);
            } else if (id == 0) {
                (*numOfCmd)++;

                if (execvp(argv[0], argv) == -1) {
                    perror("execvp");
                }
                garbageCollector(argv, sizeOfArray);

                if (strcmp(input, ENDING_WORD) != 0)
                    printf("command not found\n");

                exit(EXIT_FAILURE);

            } else {
                wait(&id);

                if (strcmp(input, ENDING_WORD) != 0) {
                    DisplayPrompt();
                } else {
                    printf(RED "Num of cmd: %d\n", *numOfCmd);
                    printf("cmd length: %d\n", *cmdLength - 4);
                    printf("End Tien's shell!\n");
                    sleep(3);
                    system("cmatrix");
                }
            }
        }
    }
    garbageCollector(argv, sizeOfArray);
    return 0;
}
// display a new shell
void DisplayPrompt() {
    struct passwd *p;
    uid_t uid = 0;
    if ((p = getpwuid(uid)) == NULL)
        perror("getpwuid() error");
    else {
        char prompt[PATH_MAX];
        if (getcwd(prompt, sizeof(prompt)) != NULL) {
            char *lastSlash = strrchr(prompt, '/');

            if (lastSlash != NULL) {
                printf(YELLOW_TEXT "%s@ubuntu:%s>" ANSI_COLOR_RESET, p->pw_name, lastSlash + 1);
            } else {
                printf(YELLOW_TEXT "%s@ubuntu:%s>" ANSI_COLOR_RESET, p->pw_name, prompt);
            }
        } else {
            perror("getcwd");
        }
    }

    fflush(stdout); // Ensure the prompt is displayed immediately
}

void garbageCollector(char **argv, int size) {
    for (int i = 0; i < size; ++i) {
        free(argv[i]);
    }
    free(argv);
    argv = NULL;
}

char** execFunction(char *input, char **argv, int *sizeOfArray, int *cmdLength) {
    int i = 0, counter = 0;
    char inputCopy[INPUT_SIZE];
    strcpy(inputCopy, input);

    char *ptr = strtok(input, CUTTING_WORD);
    while (ptr != NULL) {
        ptr = strtok(NULL, CUTTING_WORD);
        counter++;
    }

    argv = (char **)malloc((counter + 1) * sizeof(char *));
    if (argv == NULL) {
        perror("malloc");
        exit(0);
    }

    char *ptrCopy = strtok(inputCopy, CUTTING_WORD);
    while (ptrCopy != NULL) {
        if (i == 0)
            (*cmdLength) += strlen(ptrCopy);

        argv[i] = strdup(ptrCopy);
        if (argv[i] == NULL) {
            perror("malloc");
            for (int j = i - 1; j >= 0; j--) {
                free(argv[j]);
            }
            free(argv);
            exit(0);
        }

        ptrCopy = strtok(NULL, CUTTING_WORD);
        i++;
    }

    argv[counter] = NULL;
    (*sizeOfArray) = counter;
    return argv;
}
void media(){
	system("fortune | lolcat");
}

void run_cowsay(const char *input) {
    char command[256];
    // Xây dựng lệnh cowsay với chuỗi đầu vào
    sprintf(command, "cowsay \"%s\"", input);
    // Chạy lệnh cowsay
    system(command);
}

void init_shell() {
    system("clear");
    system("figlet -c 'MY SHELL'");
    printf("\n\n\n\n******************"
        "****************************************************");
    printf("\n\n\n\t            -WELCOME TO MY SHELL-");
    printf("\n\n\n\n*******************"
        "****************************************************");
    char *username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(1);
    system("clear");
}
// show ip infomation 
void handleIpInfo() {
    printf("Executing ipinfo command...\n");
    system("curl ipinfo.io"); // You may need to install curl if not already installed.
    printf("\n");
}
// show help command
void helpcommand() {
    FILE *lolcat_pipe = popen("lolcat", "w");
    if (!lolcat_pipe) {
        perror("Error opening lolcat pipe");
        exit(EXIT_FAILURE);
    }

    fprintf(lolcat_pipe, "\n*************** HELP ***************");
    fprintf(lolcat_pipe, "\n help  - research for shell");
    fprintf(lolcat_pipe, "\n mode  - mode function");
    fprintf(lolcat_pipe, "\n chat  - open chat application");
    fprintf(lolcat_pipe, "\n time  - show a current time");
    fprintf(lolcat_pipe, "\n day   - show a calendar");
    fprintf(lolcat_pipe, "\n ipinfo- show a ip information");
    fprintf(lolcat_pipe, "\n camera- open a camera");
    fprintf(lolcat_pipe, "\n game  - open a game (guess number)");
    fprintf(lolcat_pipe, "\n git   - show status of git's project");
    fprintf(lolcat_pipe, "\n github- show repo of nlhtien");
    fprintf(lolcat_pipe, "\n system- show information system");
    fprintf(lolcat_pipe, "\n*************** FUNNY **************");
    fprintf(lolcat_pipe, "\n say   - show cowsay");
    fprintf(lolcat_pipe, "\n media - show fortune");
    fprintf(lolcat_pipe, "\n xeyes - show eyes");
    fprintf(lolcat_pipe, "\n aafire- show fire");
    fprintf(lolcat_pipe, "\n oneko - show cat run");
    fprintf(lolcat_pipe, "\n ncdu  - show disk and enter 'q' to quit");
    fprintf(lolcat_pipe, "\n*******************"
        "***********************\n");

    pclose(lolcat_pipe);
}
// chat application
void chatcommand() {
    int chatInput;
    int exitChat = 0;
    system("clear");

    while (!exitChat) {
        //system("clear");
        system("toilet -f mono12 -F metal CHAT");
        printf("\n*****CHAT APPLICATION*****\n");
        printf("1. Show IP connect\n");
        printf("2. socket \n");
        printf("3. Go back\n");
        printf("Enter chat option (1-3): ");

        char chatInputStr[INPUT_SIZE2];
        fgets(chatInputStr, INPUT_SIZE2, stdin);
        if (sscanf(chatInputStr, "%d", &chatInput) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            system("clear");
            continue;
        }

        switch (chatInput) {
            case 1:
                subFunctionChat1();
                break;
            case 2:
                subFunctionChat2();
                break;
            case 3:
                exitChat = 1;
                system("clear");
                break;
            default:
                printf(RED "Invalid chat option.\n" ANSI_COLOR_RESET);
                system("clear");
                break;
        }
    }
}

void subFunctionChat1() {
    // Mở lệnh 'ss -tulpn' để đọc thông tin
    FILE *fp = popen("ss -tulpn", "r");
    if (fp == NULL) {
        perror("popen failed");
        return;
    }

    // Đọc và in thông tin từ đầu ra của lệnh 'ss -tulpn'
    printf(ANSI_COLOR_GREEN "%-10s %-15s %-10s %-20s:%-5s\n" ANSI_COLOR_RESET, "Netid", "Recv-Q", "Send-Q", "Local Address", "Port");
    printf(ANSI_COLOR_GREEN "------------------------------------------------------------\n" ANSI_COLOR_RESET);

    char buffer[256];
    // Bỏ qua hai dòng đầu tiên của kết quả vì chúng không chứa thông tin cần thiết
    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        char netid[10], state[10], recvQ[10], sendQ[10], localAddrPort[30], peerAddrPort[30], process[50];

        // Phân tích dữ liệu từ buffer (tuỳ thuộc vào định dạng của lệnh 'ss -tulpn')
        sscanf(buffer, "%10s %10s %10s %10s %30s %30s %50s", netid, state, recvQ, sendQ, localAddrPort, peerAddrPort, process);

        // Kiểm tra nếu state là "LISTEN" và chỉ in ra thông tin nếu đúng
        if (strcmp(state, "LISTEN") == 0) {
            // Tách địa chỉ IP và cổng từ localAddrPort
            char *localAddr = strtok(localAddrPort, ":");
            char *port = strtok(NULL, ":");  // Thay đổi ở đây

            // In ra thông tin cần thiết, bạn có thể tùy chỉnh theo ý muốn
            printf(ANSI_COLOR_GREEN "%-10s %-15s %-10s %-20s:%-5s\n" ANSI_COLOR_RESET, netid, recvQ, sendQ, localAddr, port);
        }
    }

    // Đóng file pointer
    pclose(fp);
}

void subFunctionChat2() {
    printf("\n************************************\n");
    printf("Executing Sub-function 2 in Chat...\n");

    pthread_t connectThread2;
    if (pthread_create(&connectThread2, NULL, connectThread, NULL) != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    pthread_join(connectThread2, NULL);

    printf("\n************************************\n");
}

void handleMode() {
    int modeInput;
    int exitMode = 0;
    system("clear");

    while (!exitMode) {
        system("toilet -f mono12 -F metal MODE");
        printf("\n*************** Mode ***************");
        printf("\n1. Note manager");
        printf("\n2. Music");
        printf("\n3. Weather");  // Tùy chọn mới
        printf("\n4. Calculator");  // Tùy chọn mới
        printf("\n5. Go back");
        printf("\nEnter mode option (1-5): ");

        char modeInputStr[INPUT_SIZE2];
        fgets(modeInputStr, INPUT_SIZE2, stdin);
        if (sscanf(modeInputStr, "%d", &modeInput) != 1) {
            printf(RED "Invalid input. Please enter a valid number.\n" ANSI_COLOR_RESET);
            system("clear");
            continue;
        }

        switch (modeInput) {
            case 1:
                subFunction1();
                break;
            case 2:
                subFunction2();
                break;
            case 3:
                subFunction3();  // Gọi hàm cho tùy chọn mới
                break;
            case 4:
                subFunction4();  // Gọi hàm cho tùy chọn mới
                break;
            case 5:
                exitMode = 1;
                system("clear");
                break;
            default:
                printf(RED "Invalid mode option.\n" ANSI_COLOR_RESET);
                sleep(2);
                system("clear");
                break;
        }
    }
}

void subFunction1() {
    printf("\n************************************\n");
    printf("Executing Sub-function 1...\n");

    int backOption;
    printf("\nDo you want to go back? (1 for yes, 0 for no): ");
    scanf("%d", &backOption);

    if (backOption == 1) {
        system("clear");
        return;
    }
    // Initialize your program, load tasks from file
    loadTasksFromFile();
    manageTasks();
    // Save tasks to file before exiting
    saveTasksToFile();
}

void subFunction2() {
    system("clear");
    printf(RED "\n**********************************************************\n" ANSI_COLOR_RESET);
    printf(RED "Executing Sub-function 2...................................\n" ANSI_COLOR_RESET);

    while (1) {
        char songName[100];
        printf("Enter the name of the song: ");
        fgets(songName, sizeof(songName), stdin);
        songName[strcspn(songName, "\n")] = '\0';  // Remove newline character if present
        
        // Thêm chức năng nhập thời gian nghe
        int listenTime;
        printf("Enter listening time in seconds: ");
        if (scanf("%d", &listenTime) != 1) {
		fprintf(stderr,RED "Invalid input for listening time.\n" ANSI_COLOR_RESET);
		return;
        }

        // Clear the input buffer
        int c2;
        while ((c2 = getchar()) != '\n' && c2 != EOF);

        // Thêm chức năng chơi âm nhạc ở đây
        playMusic(songName,listenTime);

        int backOption;
        // Sử dụng sscanf để chuyển đổi chuỗi nhập vào thành giá trị cho backOption
        printf("\nDo you want to go back? (1 for yes, enter for no): ");
        if (fgets(songName, sizeof(songName), stdin) == NULL || sscanf(songName, "%d", &backOption) != 1) {
            fprintf(stderr,RED "Invalid input.\n" ANSI_COLOR_RESET);
            continue;  // Bắt đầu lại vòng lặp nếu có lỗi
        }

	// Clear the input buffer
	int c;
	while ((c = getchar()) != '\n' && c != EOF);

        if (backOption == 1) {
            system("clear");
            break;  // Thoát khỏi vòng lặp nếu người dùng chọn quay lại
        }
    }
}


void subFunction3() {
    system("clear");
    printf("\n************************************\n");
    printf("Executing Sub-function 3...\n");
    int backOption;
    get_weather_info();
    printf("\nDo you want to go back? (1 for yes): ");
    scanf("%d", &backOption);
    
    // Call the function to get weather information


    if (backOption == 1) {
        system("clear");
        return;
    }

}

void subFunction4() {
    system("clear");
    do {
        system("clear");
        printf("\n************************************\n");
        printf("Executing Sub-function 4 (Simple Calculator)...\n");

        int choice;
        double num1, num2, result;

        printf("Select operation:\n");
        printf("1. Addition\n");
        printf("2. Subtraction\n");
        printf("3. Multiplication\n");
        printf("4. Division\n");
        printf("Enter choice (1-4): ");
        scanf("%d", &choice);

        printf("Enter first number: ");
        scanf("%lf", &num1);

        printf("Enter second number: ");
        scanf("%lf", &num2);

        switch (choice) {
            case 1:
                result = num1 + num2;
                printf("Result: %.2lf\n", result);
                break;
            case 2:
                result = num1 - num2;
                printf("Result: %.2lf\n", result);
                break;
            case 3:
                result = num1 * num2;
                printf("Result: %.2lf\n", result);
                break;
            case 4:
                if (num2 != 0) {
                    result = num1 / num2;
                    printf("Result: %.2lf\n", result);
                } else {
                    printf(RED "Error: Division by zero is not allowed.\n" ANSI_COLOR_RESET);
                }
                break;
            default:
                printf(RED "Invalid choice\n" ANSI_COLOR_RESET);
                sleep(1);
                break;
        }

        printf("************************************\n");

        int backOption;
        printf("\nDo you want to go back? (1 for yes, 0 for no): ");
        scanf("%d", &backOption);

        if (backOption == 1) {
            system("clear");
            return;
        }

    } while (1);  // Vòng lặp sẽ lặp lại vô hạn cho đến khi người dùng chọn quay lại
}


void* connectThread(void* arg) {
    char serverIP[INPUT_SIZE];
    int serverPort;

    printf("Enter server IP: ");
    fgets(serverIP, INPUT_SIZE, stdin);

    size_t len = strlen(serverIP);
    if (len > 0 && serverIP[len - 1] == '\n') {
        serverIP[len - 1] = '\0';
    }

    char portStr[INPUT_SIZE];
    printf("Enter server port: ");
    fgets(portStr, sizeof(portStr), stdin);

    if (sscanf(portStr, "%d", &serverPort) != 1) {
    	fprintf(stderr, "Invalid port input.\n");
    	return NULL;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Socket creation failed");
        return NULL;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverIP, &serverAddr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(clientSocket);
        return NULL;
    }

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(clientSocket);
        return NULL;
    }

    printf("Connected to server (%s:%d)\n", serverIP, serverPort);

    pthread_t clientThread;
    if (pthread_create(&clientThread, NULL, handleClient, &clientSocket) != 0) {
        perror("Thread creation failed");
        close(clientSocket);
        return NULL;
    }

    pthread_detach(clientThread);

    // Không cần pthread_join ở đây nếu bạn đã detach thread.

    return NULL;
}


void *handleClient(void *arg) {
    int clientSocket = *((int *)arg);

    char buffer[1024];
    int bytesRead = read(clientSocket, buffer, sizeof(buffer));
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("Received from server: %s", buffer);
    }

    close(clientSocket);

    return NULL;
}
void saveTasksToFile() {
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < numTasks; i++) {
        fprintf(file, "%s\n%s\n%s\n", tasks[i].name, tasks[i].description, tasks[i].deadline);
    }

    fclose(file);
}

void loadTasksFromFile() {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("No existing tasks found.\n");
        return;
    }

    numTasks = 0;
    while (fgets(tasks[numTasks].name, sizeof(tasks[numTasks].name), file) != NULL) {
        fgets(tasks[numTasks].description, sizeof(tasks[numTasks].description), file);
        fgets(tasks[numTasks].deadline, sizeof(tasks[numTasks].deadline), file);
        tasks[numTasks].name[strcspn(tasks[numTasks].name, "\n")] = '\0';
        tasks[numTasks].description[strcspn(tasks[numTasks].description, "\n")] = '\0';
        tasks[numTasks].deadline[strcspn(tasks[numTasks].deadline, "\n")] = '\0';
        numTasks++;
    }

    fclose(file);
}

void displayTasks() {
    loadTasksFromFile();

    printf(ANSI_COLOR_GREEN "\n*************** Task List ***************\n" ANSI_COLOR_RESET);
    for (int i = 0; i < numTasks; i++) {
        printf("%d. Name: %s\n   Description: %s\n   Deadline: %s\n", i + 1, tasks[i].name, tasks[i].description, tasks[i].deadline);
    }
    printf("*****************************************\n");
}

void addTask() {
    Task newTask;
    printf("Enter task name: ");
    fgets(newTask.name, sizeof(newTask.name), stdin);
    newTask.name[strcspn(newTask.name, "\n")] = '\0';

    printf("Enter task description: ");
    fgets(newTask.description, sizeof(newTask.description), stdin);
    newTask.description[strcspn(newTask.description, "\n")] = '\0';

    printf("Enter task deadline: ");
    fgets(newTask.deadline, sizeof(newTask.deadline), stdin);
    newTask.deadline[strcspn(newTask.deadline, "\n")] = '\0';

    tasks[numTasks++] = newTask;
    printf(ANSI_COLOR_GREEN "Task added successfully!\n" ANSI_COLOR_RESET);
    saveTasksToFile();

}

void removeTask(int index) {
    if (index >= 0 && index < numTasks) {
        for (int i = index; i < numTasks - 1; i++) {
            tasks[i] = tasks[i + 1];
        }
        numTasks--;
        printf(ANSI_COLOR_GREEN "Task removed successfully!\n" ANSI_COLOR_RESET);
    } else {
        printf("Invalid task index!\n");
    }
    
    saveTasksToFile();
}

void manageTasks() {
    int taskInput;
    int exitTask = 0;
    system("clear");

    while (!exitTask) {
        printf("\n*************** Task Mode ***************\n");
        printf("1. Display Tasks\n");
        printf("2. Add Task\n");
        printf("3. Remove Task\n");
        printf("4. Go back\n");
        printf("Enter task option (1-4): ");

        char taskInputStr[10];
        fgets(taskInputStr, sizeof(taskInputStr), stdin);
        if (sscanf(taskInputStr, "%d", &taskInput) != 1) {
            printf(RED "Unsucessfully pick!!!!\n" ANSI_COLOR_RESET);
            system("clear");
            continue;
        }

        switch (taskInput) {
            case 1:
                system("clear");
                displayTasks();
                break;
            case 2:
                system("clear");
                addTask();
                break;
            case 3:
                system("clear");
                int removeIndex;
                char removeIndexStr[INPUT_SIZE2];
                printf("Enter the index to remove: ");
                fgets(removeIndexStr, sizeof(removeIndexStr), stdin);
                sscanf(removeIndexStr, "%d", &removeIndex);
                removeTask(removeIndex - 1);
                getchar(); // Consume the newline character
                break;
            case 4:
                exitTask = 1;
                system("clear");
                break;
            default:
                printf(RED "Invalid task option.\n" ANSI_COLOR_RESET);
                sleep(2);
                system("clear");
                break;
        }
    }
}

void playMusic(const char *songName, int listenTimeInSeconds) {
    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;

    char filename[256];
    snprintf(filename, sizeof(filename), "%s.wav", songName);  // Assume all songs are in WAV format

    if (SDL_LoadWAV(filename, &wavSpec, &wavBuffer, &wavLength) == NULL) {
        fprintf(stderr,RED "Could not load audio file: %s\n" ANSI_COLOR_RESET, SDL_GetError());
        return;
    }

    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    if (deviceId == 0) {
        fprintf(stderr,RED "Failed to open audio device: %s\n" ANSI_COLOR_RESET, SDL_GetError());
        SDL_FreeWAV(wavBuffer);
        return;
    }

    SDL_PauseAudioDevice(deviceId, 0);

    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    
    // Thời gian chờ được thay đổi dựa trên tham số listenTimeInSeconds
    SDL_Delay(listenTimeInSeconds * 1000); // Chuyển đổi thời gian từ giây sang mili giây

    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
    SDL_Quit();
}

// Callback function for writing received data
size_t write_callback(void *contents, size_t size, size_t nmemb, char **output) {
    size_t realsize = size * nmemb;
    *output = realloc(*output, strlen(*output) + realsize + 1);
    if (*output == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    strcat(*output, contents);
    return realsize;
}

// Function to get weather information
void get_weather_info() {
    CURL *curl;
    CURLcode res;
    char *response = malloc(1);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        // Construct the API URL with your API key and city name
        char url[100];
        sprintf(url, "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s", CITY_NAME, API_KEY);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Parse and display the received JSON response (you may want to use a JSON parsing library)
        printf("Weather Information:\n%s\n", response);

        // Clean up
        free(response);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void handleday() {
    // Lấy thông tin về thời gian hiện tại
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);

    // In thông tin về ngày hiện tại
    printf("Current date and time: %s", asctime(info));
}

void handleDay2() {
    int year, month;

    char yearStr[INPUT_SIZE2];
    printf("Enter year: ");
    fgets(yearStr, sizeof(yearStr), stdin);
    sscanf(yearStr, "%d", &year);

    char monthStr[INPUT_SIZE2];
    printf("Enter month (1-12): ");
    fgets(monthStr, sizeof(monthStr), stdin);
    sscanf(monthStr, "%d", &month);


    displayCalendar(year, month);
}

void displayCalendar(int year, int month) {
    struct tm timeinfo = {0};
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = 1;

    time_t rawtime = mktime(&timeinfo);
    struct tm *info = localtime(&rawtime);

    printf("       %s %d       \n", (month == 1) ? "January" :
                                     (month == 2) ? "February" :
                                     (month == 3) ? "March" :
                                     (month == 4) ? "April" :
                                     (month == 5) ? "May" :
                                     (month == 6) ? "June" :
                                     (month == 7) ? "July" :
                                     (month == 8) ? "August" :
                                     (month == 9) ? "September" :
                                     (month == 10) ? "October" :
                                     (month == 11) ? "November" : "December", year);

    printf(RED BG_WHITE " Su  Mo  Tu  We  Th  Fr  Sa\n" ANSI_COLOR_RESET);

    for (int i = 0;i < (info->tm_wday + 7) % 7; i++) {
        printf("    ");
    }

    int daysInMonth = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        daysInMonth = 30;
    } else if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            daysInMonth = 29;
        } else {
            daysInMonth = 28;
        }
    }

    for (int day = 1; day <= daysInMonth; day++) {
        printf(ANSI_COLOR_GREEN "%3d" ANSI_COLOR_RESET, day);
        if ((info->tm_wday + day) % 7 == 0) {
            printf("\n");
        } else {
            printf(" ");
        }
    }

    printf("\n");
}

void handlecamera() {
//     // Mở camera
//     cv::VideoCapture cap(0);
//     if (!cap.isOpened()) {
//         std::cerr << "Error: Unable to open the camera.\n";
//         return;
//     }

//     // Kiểm tra và lấy thông số kỹ thuật của camera
//     int frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
//     int frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

//     // Tạo cửa sổ để hiển thị hình ảnh từ camera
//     cv::namedWindow("Camera", cv::WINDOW_NORMAL);
//     cv::resizeWindow("Camera", frameWidth, frameHeight);

//     // Chuẩn bị biến để lưu ảnh từ camera
//     cv::Mat frame;

//     // Hiển thị hình ảnh từ camera và chụp hình khi nhấn 'c'
//     char key = 0;
//     while (key != 'q') {
//         cap.read(frame);
//         if (frame.empty()) {
//             std::cerr << "Error: Unable to capture frame.\n";
//             break;
//         }

//         // Hiển thị hình ảnh từ camera
//         cv::imshow("Camera", frame);

//         // Chụp hình khi nhấn 'c'
//         key = cv::waitKey(1);
//         if (key == 'c') {
//             // Lưu ảnh với tên file duy nhất dựa trên thời gian hiện tại
//             std::string filename = "captured_image_" + std::to_string(time(0)) + ".png";
//             cv::imwrite(filename, frame);
//             std::cout << "Image captured and saved as: " << filename << std::endl;
//         }
//     }

//     // Đóng cửa sổ và giải phóng tài nguyên
//     cv::destroyWindow("Camera");
//     cap.release();
}

void handlegame() {
    printf("\n************************************\n");
    printf("Executing Game Mode...\n");

    // Seed for random number generation
    srand(time(NULL));

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Generate the secret number
    generateSecretNumber();
    system("clear");

    // Create a thread for guessing
    pthread_t guessThreadId;
    if (pthread_create(&guessThreadId, NULL, guessThread, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    //system("clear");



    // Main game loop
    while (!exitGame) {
        // Read guess from the pipe
        read(pipefd[0], &guess, sizeof(guess));

        // Check if the guess is correct
        if (guess == secretNumber) {
            printf("Congratulations! You guessed the correct number: %d\n", secretNumber);
            printf("Total attempts: %d\n", attempts);
            printf(ANSI_COLOR_GREEN "\n\n                      GOODJOB BRO !!! \n\n" ANSI_COLOR_RESET);
            sleep(5);
            break;
        } else if (guess==0) {
            break; // Exit the loop if exitGame is set
        } else {
            // Provide hint
            if (guess < secretNumber) {
                printf("\nWrong guess. The secret number is greater than %d. Try again!\n", guess);
                printf("\n**************************************************************");
            } else {
                printf("\nWrong guess. The secret number is smaller than %d. Try again!\n", guess);
                printf("\n**************************************************************");   
            }
        }
    }
    // Clean up
    close(pipefd[0]);
    close(pipefd[1]);
    // Wait for the guess thread to finish
    pthread_join(guessThreadId, NULL);
    printf("\n*********************************END*****************************\n");
    sleep(2);
    system("clear");
}

// Function to generate a random number
void generateSecretNumber() {
    secretNumber = rand() % MAX_GUESS + 1;
}

// Function executed by the guess thread
void* guessThread(void* arg) {
    //int* exitGame = (int*)arg;
    while (1) {
        printf("****************************************************************\n");
        printf(RED "          Welcome to the Number Guessing Game!\n" ANSI_COLOR_RESET);
        printf(RED "     Try to guess the secret number between 1 and %d.\n" ANSI_COLOR_RESET, MAX_GUESS);
        printf("Enter your guess (1-%d), or enter 0 to exit: ", MAX_GUESS);

        char guessStr[INPUT_SIZE2];
	fgets(guessStr, sizeof(guessStr), stdin);
	sscanf(guessStr, "%d", &guess);
        // Check for exit option
        /*if (guess == 0) {
            printf("Exiting the game...\n");
            *exitGame = 1;
            break;
        }*/
        // Write guess to the pipe
        write(pipefd[1], &guess, sizeof(guess));
        attempts++;
        sleep(2);
        if(guess ==0){
        	break;
        }
        system("clear");
    }
    return NULL;
}

void git_info() {
    FILE *fp;
    char buffer[1035];

    // Git branch
    fp = popen("git rev-parse --abbrev-ref HEAD 2>/dev/null", "r");
    if (fp != NULL) {
        fgets(buffer, sizeof(buffer)-1, fp);
        printf("Branch: %s", buffer);
        pclose(fp);
    }

    // Git status
    printf("\n\nStatus:\n");
    fp = popen("git status --porcelain", "r");
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer)-1, fp) != NULL) {
            printf("%s", buffer);
        }
        pclose(fp);
    }

    // Git commit info
    printf("\n\nCommit Information:\n");
    fp = popen("git log --oneline --graph --decorate --all", "r");
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer)-1, fp) != NULL) {
            printf("%s", buffer);
        }
        pclose(fp);
    }
}
// Callback function for libcurl to handle the API response
size_t write_callback2(void *contents, size_t size, size_t nmemb, void *userp) {
    // Tạo một bộ đệm để lưu trữ dữ liệu nhận được từ phản hồi API
    char *buffer = (char *)contents;

    // Initialize a JSON object
    json_t *root;
    json_error_t error;

    // Load the response into a JSON object
    root = json_loads(buffer, 0, &error);
    printf("Raw JSON: %s\n", json_dumps(root, JSON_ENCODE_ANY));

    if (root) {
        // Check if the JSON object is an array
        if (json_is_array(root)) {
            // Duyệt qua từng mục trong danh sách
            size_t i;
            for (i = 0; i < json_array_size(root); i++) {
                json_t *repo = json_array_get(root, i);

                // Lấy giá trị của trường "id" và "full_name"
                const char *id = json_string_value(json_object_get(repo, "id"));
                const char *full_name = json_string_value(json_object_get(repo, "full_name"));

                // In ra thông tin
                printf("Repository %lu:\n", i + 1);
                printf("ID: %s\n", id ? id : "N/A");
                printf("Full Name: %s\n", full_name ? full_name : "N/A");
                printf("\n");
            }
        }

        // Giải phóng bộ nhớ
        json_decref(root);
    } else {
        // In ra thông báo lỗi nếu có
        fprintf(stderr, "Error parsing JSON: %s\n", error.text);
    }

    return size * nmemb;
}

size_t write_callback3(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    printf("%.*s", (int)realsize, (char *)contents);  // Print raw response

    return realsize;
}

void get_github_status(const char *username) {
    // Initialize libcurl
    CURL *curl = curl_easy_init();

    if (curl) {
        // Construct the API URL for user's repositories
        char url[100];
        snprintf(url, sizeof(url), "https://api.github.com/users/%s/repos", username);

        // Set up the libcurl request
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback2);

        // Set the User-Agent header
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "YourApp/1.0");

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Clean up
        curl_easy_cleanup(curl);
    }
}

// Hàm hiển thị thông tin CPU
void display_cpu_info() {
    FILE* cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo == NULL) {
        perror("Error opening /proc/cpuinfo");
        exit(EXIT_FAILURE);
    }

    char line[256];
    int cpu_cores = 0;
    char cpu_model[256];
    while (fgets(line, sizeof(line), cpuinfo)) {
        if (strncmp(line, "processor", 9) == 0) {
            cpu_cores++;
        } else if (strncmp(line, "model name", 10) == 0) {
            char* pos = strchr(line, ':');
            if (pos != NULL) {
                strncpy(cpu_model, pos + 2, sizeof(cpu_model));
                break;
            }
        }
    }
    fclose(cpuinfo);

    printf(ANSI_COLOR_BLUE BG_WHITE "CPU Information:" ANSI_COLOR_RESET);
    printf("\n");
    printf(ANSI_COLOR_BLUE "|  - Cores: %d\n", cpu_cores);
    printf("|  - Model: %s\n" ANSI_COLOR_RESET, cpu_model);
}

// Hàm hiển thị thông tin ổ đĩa
void display_disk_info() {
    struct statvfs fs_info;
    if (statvfs("/", &fs_info) == -1) {
        perror("Error getting disk space information");
        exit(EXIT_FAILURE);
    }

    printf(ANSI_COLOR_BLUE BG_WHITE "Disk Information:" ANSI_COLOR_RESET);
    printf("\n");
    printf(ANSI_COLOR_BLUE "|  - Total disk space: %.2f GB\n", (double)fs_info.f_blocks * fs_info.f_frsize / (1024 * 1024 * 1024));
    printf("|  - Free disk space: %.2f GB\n", (double)fs_info.f_bfree * fs_info.f_frsize / (1024 * 1024 * 1024));
    printf("|  - Available disk space: %.2f GB\n" ANSI_COLOR_RESET, (double)fs_info.f_bavail * fs_info.f_frsize / (1024 * 1024 * 1024));
}

// Hàm hiển thị thông tin hệ thống
void display_system_info() {
    struct utsname sys_info;
    struct sysinfo sys_usage;

    // Lấy thông tin hệ thống
    if (uname(&sys_info) == -1) {
        perror("Error getting system information");
        exit(EXIT_FAILURE);
    }

    // Lấy thông tin sử dụng hệ thống
    if (sysinfo(&sys_usage) == -1) {
        perror("Error getting system usage information");
        exit(EXIT_FAILURE);
    }

    // In thông tin hệ thống
    printf(ANSI_COLOR_BLUE BG_WHITE "System Information:" ANSI_COLOR_RESET);
    printf("\n");
    printf(ANSI_COLOR_BLUE "|  - OS: %s\n", sys_info.sysname);
    printf("|  - Kernel version: %s\n", sys_info.release);
    printf("|  - Node name: %s\n", sys_info.nodename);
    printf("|  - Machine: %s\n", sys_info.machine);
    printf("|  - Processor: %s\n", sys_info.machine);
    printf("|  - Uptime: %ld days, %ld:%02ld:%02ld\n", 
           sys_usage.uptime / (60 * 60 * 24),
           (sys_usage.uptime / (60 * 60)) % 24,
           (sys_usage.uptime / 60) % 60,
           sys_usage.uptime % 60);
    printf("|  - Total RAM: %lu MB\n", sys_usage.totalram / (1024 * 1024));
    printf("|  - Free RAM: %lu MB\n", sys_usage.freeram / (1024 * 1024));
    printf("|  - Total swap: %lu MB\n", sys_usage.totalswap / (1024 * 1024));
    printf("|  - Free swap: %lu MB\n" ANSI_COLOR_RESET, sys_usage.freeswap / (1024 * 1024));
}


void parseRss(const char *url) {
    xmlDocPtr doc;
    xmlNodePtr node;
    doc = xmlReadFile(url, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse document\n");
        return;
    }
    node = xmlDocGetRootElement(doc);
    if (node == NULL) {
        fprintf(stderr, "Empty document\n");
        xmlFreeDoc(doc);
        return;
    }
    if (xmlStrcmp(node->name, (const xmlChar *) "rss")) {
        fprintf(stderr, "Document of the wrong type, root node != rss\n");
        xmlFreeDoc(doc);
        return;
    }
    for (node = node->children; node != NULL; node = node->next) {
        if (node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *) "channel")) {
            for (xmlNodePtr itemNode = node->children; itemNode != NULL; itemNode = itemNode->next) {
                if (itemNode->type == XML_ELEMENT_NODE && !xmlStrcmp(itemNode->name, (const xmlChar *) "item")) {
                    for (xmlNodePtr dataNode = itemNode->children; dataNode != NULL; dataNode = dataNode->next) {
                        if (dataNode->type == XML_ELEMENT_NODE) {
                            xmlChar *content = xmlNodeListGetString(doc, dataNode->children, 1);
                            if (content) {
                                printf("%s: %s\n", dataNode->name, content);
                                xmlFree(content);
                            }
                        }
                    }
                    printf("\n");
                }
            }
            break;
        }
    }
    xmlFreeDoc(doc);
}

// Callback function để ghi dữ liệu nhận được từ SMTP server
static size_t write_callback4(void *ptr, size_t size, size_t nmemb, void *stream) {
    return fwrite(ptr, size, nmemb, (FILE *)stream);
}

// Hàm gửi email
void send_email(const char *to, const char *subject, const char *body) {
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;

    // Khởi tạo thư viện curl
    curl = curl_easy_init();
    if (curl) {
        // Thiết lập các thông tin cần thiết cho gửi email
        curl_easy_setopt(curl, CURLOPT_USERNAME, "hoangkenzi2002@gmail.com");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "shdgajs");
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "hoangkenzi2002@gmail.com");

        // Thêm người nhận email
        recipients = curl_slist_append(recipients, to);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        // Tạo body email
        FILE *body_file = fmemopen((char *)body, strlen(body), "r");
        curl_easy_setopt(curl, CURLOPT_READDATA, body_file);

        // Callback để ghi dữ liệu nhận được từ SMTP server
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback4);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // Hiển thị thông tin debug

        // Gửi email
        res = curl_easy_perform(curl);

        // Giải phóng bộ nhớ và dọn dẹp
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
        fclose(body_file);
    }

    // Kiểm tra kết quả gửi email
    if (res != CURLE_OK) {
        fprintf(stderr, "Failed to send email: %s\n", curl_easy_strerror(res));
    } else {
        printf("Email sent successfully!\n");
    }
}

