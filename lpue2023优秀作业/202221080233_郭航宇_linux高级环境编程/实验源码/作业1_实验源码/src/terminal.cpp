#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include "filesystem.hpp"

class Terminal
{
private:
    FileSystem *fs;
    // 使用空格来分割字符串，并返回一个字符串向量
    std::vector<std::string> split(const std::string &str)
    {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;

        while (ss >> token)
        {
            tokens.push_back(token);
        }

        return tokens;
    }
    std::vector<std::string> getContent(const std::string &str)
    {
        std::vector<std::string> tokens;
        std::regex re("\"([^\"]*)\"");
        std::sregex_iterator it(str.begin(), str.end(), re);
        std::sregex_iterator end;

        while (it != end)
        {
            tokens.push_back(it->str(1));
            ++it;
        }

        return tokens;
    }

public:
    Terminal()
    {
        fs = new FileSystem();
        std::cout << "Welcome to the file system!" << std::endl;
    }
    void run()
    {
        while (true)
        {
            std::cout << "$ "; // prompt
            std::string input;
            getline(std::cin, input);

            // 处理输入指令及参数
            auto command = split(input);

            if (command.empty())
                continue;

            // 如果用户输入 "exit"，则退出
            if (command[0] == "exit")
            {
                delete fs;
                break;
            }
            else if (command[0] == "create")
            {
                try
                {
                    if (command.size() > 2)
                    {
                        throw std::invalid_argument("Too many arguments");
                    }
                    if (command.size() < 2)
                    {
                        throw std::invalid_argument("Need a filename");
                    }
                }
                catch (const std::invalid_argument &e)
                {
                    errorMessage(e);
                    errorMessage("Usage: create <filename>");
                    continue;
                }
                fs->create(command[1]);
            }
            else if (command[0] == "delete")
            {
                try
                {
                    if (command.size() > 2)
                    {
                        throw std::invalid_argument("Too many arguments");
                    }
                    if (command.size() < 2)
                    {
                        throw std::invalid_argument("Need a filename");
                    }
                }
                catch (const std::invalid_argument &e)
                {
                    errorMessage(e);
                    errorMessage("Usage: delete <filename>");
                    continue;
                }
                fs->del(command[1]);
            }
            else if (command[0] == "read")
            {
                try
                {
                    if (command.size() > 2)
                    {
                        throw std::invalid_argument("Too many arguments");
                    }
                    if (command.size() < 2)
                    {
                        throw std::invalid_argument("Need a filename");
                    }
                }
                catch (const std::invalid_argument &e)
                {
                    errorMessage(e);
                    errorMessage("Usage: read <filename>");
                    continue;
                }
                fs->read(command[1]);
            }
            else if (command[0] == "write")
            {
                try
                {
                    if (command.size() < 3)
                    {
                        throw std::invalid_argument("Need a filename and content");
                    }
                    if (command[2][0] != '\"' || command[command.size() - 1][command[command.size() - 1].size() - 1] != '\"')
                    {
                        throw std::invalid_argument("content must be surrounded by double quotes");
                    }
                }
                catch (const std::invalid_argument &e)
                {
                    errorMessage(e);
                    errorMessage("Usage: read <filename> \"<content>\"");
                    continue;
                }
                std::vector<std::string> content = getContent(input);
                try
                {
                    if (content.size() > 1)
                    {
                        throw std::invalid_argument("Too many contents");
                    }
                }
                catch (const std::invalid_argument &e)
                {
                    errorMessage(e);
                    errorMessage("Usage: read <filename> \"<content>\"");
                    continue;
                }
                std::cout << content[0] << std::endl;
                fs->write(command[1], content[0]);
            }
            else if (command[0] == "list")
            {
                try
                {
                    if (command.size() > 1)
                    {
                        throw std::invalid_argument("Too many arguments");
                    }
                }
                catch (const std::invalid_argument &e)
                {
                    errorMessage(e);
                    errorMessage("Usage: list");
                    continue;
                }
                fs->list();
            }
            else
            {
                std::cout << "Unknown command: " << command[0] << std::endl;
            }
        }
    }
};

int main()
{
    Terminal terminal;
    terminal.run();
    return 0;
}
