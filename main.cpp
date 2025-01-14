#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

int main() {

    std::string buf;
    getline(std::cin, buf);

    std::string fname = buf;

    if (buf.empty()) {
        std::cerr << "Error: empty input\n";
        exit(1);
    }
    if (buf.substr(buf.size() - 3) != ".ez") {
        std::cerr << "Error: file must be .ez\n";
        exit(1);
    }

    try {
        std::ifstream read(buf);
        std::ofstream write("output.asm");

        write << "section .text\n";
        int str_count = 0;

        while (getline(read, buf)) {
            {
                std::regex pattern (R"(^\s*stdout\((.*), (\w+)\)\s*$)");
                std::smatch match;
                if (std::regex_match(buf, match, pattern)) {
                    std::string content = match[1].str();
                    std::string argl = match[2];

                    std::string result = "";
                    for (size_t i = 0; i < content.size(); ++i) {
                        if (content[i] == '\\' && i + 1 < content.size() && content[i + 1] == '\\') {
                            result += '\\';
                            ++i;
                        } 
                        else {
                            result += content[i];
                        }
                    }

                    std::string label = "str" + std::to_string(str_count++);

                    if (content.find('"') != std::string::npos) {
                        std::string label = "str" + std::to_string(str_count++);
                        write << "section .data\n"
                            << label << ": db " << content << ", 0\n"
                            << "section .text\n"
                            << "mov rax, 1\n"
                            << "mov rdi, 1\n"
                            << "mov rsi, " << label << "\n"
                            << "mov rdx, " << argl << "\n"
                            << "syscall\n";
                    } 
                    else {
                        write << "section .text\n"
                              << "mov rax, 1\n"
                              << "mov rdi, 1\n"
                              << "mov rsi, " << content << "\n"
                              << "mov rdx, " << argl << "\n"
                              << "syscall\n";
                    }
                }
            }
            {
                std::regex pattern (R"(^\s*stdoutln\((.*), (\w+)\)\s*$)");
                std::smatch match;
                if (std::regex_match(buf, match, pattern)) {
                    std::string content = match[1].str();
                    std::string argl = match[2];

                    std::string result = "";
                    for (size_t i = 0; i < content.size(); ++i) {
                        if (content[i] == '\\' && i + 1 < content.size() && content[i + 1] == '\\') {
                            result += '\\';
                            ++i;
                        } 
                        else {
                            result += content[i];
                        }
                    }

                    std::string label = "str" + std::to_string(str_count++);

                    if (content.find('"') != std::string::npos) {
                        std::string label = "str" + std::to_string(str_count++);
                        write << "section .data\n"
                            << label << ": db " << content << ", 0\n"
                            << "section .text\n"
                            << "mov rax, 1\n"
                            << "mov rdi, 1\n"
                            << "mov rsi, " << label << "\n"
                            << "mov rdx, " << argl << "\n"
                            << "syscall\n";
                    } 
                    else {
                        write << "section .text\n"
                              << "mov rax, 1\n"
                              << "mov rdi, 1\n"
                              << "mov rsi, " << content << "\n"
                              << "mov rdx, " << argl << "\n"
                              << "syscall\n";
                    }
                }
            }
            {
                std::regex pattern(R"(^\s*stdin\((.*)\)\s*$)");
                std::smatch match;
                if (std::regex_match(buf, match, pattern)) {
                    std::string label = match[1].str();

                    write << "section .bss\n"
                          << label << ": resb 100\n"
                          << "section .text\n"
                          << "mov rax, 0\n"
                          << "mov rdi, 0\n"
                          << "mov rsi, " << label << "\n"
                          << "mov rdx, " << label.size() << "\n"
                          << "syscall\n";
                }
            }
            {
                std::regex pattern(R"(^\S+\(\S+\)$)");
                std::smatch match;
                if (std::regex_match(buf, match, pattern)) {
                    std::string label = match[1].str();
                    write << "call " << label << "\n";
                }
            }
            {
                std::regex pattern(R"(^\s*(\w+):\s+(db|dw|dq|dd|equ)\s+(.+))");
                std::smatch match;

                if (std::regex_match(buf, match, pattern)) {
                    std::string name = match[1].str();
                    std::string type = match[2].str();
                    std::string value = match[3].str();

                    if (value.find("stdin(") != std::string::npos) {
                        write << "section .bss\n"
                              << name << ": resb 100\n"
                              << "section .text\n"
                              << "mov byte [" << name <<  "+ rax], 0\n"
                              << "mov rax, 0\n"
                              << "mov rdi, 0\n"
                              << "mov rsi, " << name << "\n"
                              << "mov rdx, 255\n"
                              << "syscall\n";
                    } else {
                        if (!(type == "equ")) {
                            write << "section .data\n"
                                  << name << ": " << type << " " << value << ", 0\n"
                                  << "section .text\n";
                        }
                        else {
                            write << "section .data\n"
                                  << name << ": " << type << " " << value << "\n"
                                  << "section .text\n";
                        }
                    }
                }
            }

            std::stringstream ss(buf);
            std::string word;
            std::vector<std::string> words;

            while (ss >> word) {
                words.push_back(word);
            }

            for (int i = 0; i < words.size(); i++) {
                std::string cword = words[i];

                {    
                    std::regex pattern(R"(^&(.+):$)");
                    std::smatch match;

                    bool found = std::regex_search(cword, match, pattern);
                    if (found) {
                        std::string extracted_text = match[1].str();
                        write << "global _" << extracted_text << "\n"
                            << "_" + extracted_text + ":\n";
                    }
                }
 

                if (cword == "exit" && i + 1 < words.size()) {
                    if (words.size() > 2) {
                        std::cerr << "Error: exit() function must be the last function in the file\n";
                        exit(1);
                    }
                    write << "mov rax, 60\nmov rdi, " << words[i+1] << "\nsyscall\n";
                    break;
                }
            }
        }

        write.close();


        std::string dir = fname.substr(0, fname.size() - 3);
        {
            std::string cmd;
            #ifdef __unix__
            cmd = "mkdir -p " + dir + " && "
                  "cp output.asm " + dir + "/ && "
                  "nasm -f elf64 -o " + dir + "/output.o " + dir + "/output.asm && "
                  "ld -s -o " + dir + "/output " + dir + "/output.o && "
                  "rm " + dir + "/output.o " + dir + "/output.asm";
            /*#elif defined(_WIN32) || defined(_WIN64)
            cmd = "if (!(Test-Path -Path \"" + dir + "\")) { New-Item -ItemType Directory -Path \"" + dir + "\" } ; " 
                  "Copy-Item -Path output.asm -Destination \"" + dir + "\" ; "
                  "nasm -f win64 -o \"" + dir + "\\output.o\" \"" + dir + "\\output.asm\" ; "
                  "link /entry:_start /subsystem:console /out:\"" + dir + "\\output.exe\" \"" + dir + "\\output.o\" ; "
                  "Remove-Item \"" + dir + "\\output.o\" ; "
                  "Remove-Item \"" + dir + "\\output.asm\"";*/
            #else
            std::cerr << "Error: unsupported platform\n";
            #endif
            system(cmd.c_str());
        }
        read.close();
    } 
    catch (std::exception& ex) {
        std::cout << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}