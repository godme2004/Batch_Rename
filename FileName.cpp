#include <iostream>
#include <filesystem>
#include <regex>
#include <string>

namespace fs = std::filesystem;

// 移除路径中的双引号（如果有）
std::string clean_path(const std::string& input) 
{
    if (input.size() >= 2 && input.front() == '"' && input.back() == '"') 
    {
        return input.substr(1, input.size() - 2);
    }
    return input;
}

int main() 
{
    while (true) 
    {
        // 用户输入的目录路径
        std::string directory;
        std::cout << "请输入文件所在目录路径: ";
        std::getline(std::cin, directory);
        directory = clean_path(directory);

        // 检查目录是否存在
        if (!fs::exists(directory) || !fs::is_directory(directory)) 
        {
            std::cout << "错误: 路径无效或不是一个文件夹！" << std::endl;
            std::cout << "是否重试？(y/n): ";
            char retry;
            std::cin >> retry;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清空输入缓冲区
            if (retry == 'y' || retry == 'Y')
            {
                continue; // 重试
            }
            else 
            {
                std::cout << "程序退出。" << std::endl;
                return 1;
            }
        }

        // 用户输入的自定义文字
        std::string custom_text;
        std::cout << "请输入需要的自定义文字: ";
        std::getline(std::cin, custom_text);

        // 匹配文件名的正则表达式，捕获两位数字的序号
        std::regex pattern(R"(.*?(\d{2})(.*?)(\.[^.]+)?)");

        // 遍历目录中的文件
        bool success = false;
        for (const auto& entry : fs::directory_iterator(directory)) 
        {
            if (entry.is_regular_file()) 
            {
                std::string filename = entry.path().filename().string();
                std::smatch match;

                // 匹配文件名中的两位数字序号
                if (std::regex_match(filename, match, pattern)) 
                {
                    std::string number = match[1];    // 提取两位数字
                    std::string extension = match[3].str(); // 提取扩展名（如果存在）

                    // 构建新的文件名：自定义文字 + 下划线 + 两位数字 + 扩展名
                    std::string new_name = custom_text + "_" + number + extension;

                    // 重命名文件
                    try
                    {
                        fs::rename(entry.path(), entry.path().parent_path() / new_name);
                        std::cout << "重命名: " << filename << " -> " << new_name << std::endl;
                        success = true;
                    }
                    catch (const std::exception& e) 
                    {
                        std::cerr << "错误: 无法重命名文件 " << filename << "，原因: " << e.what() << std::endl;
                    }
                }
                else
                {
                    std::cout << "跳过: " << filename << "（未找到两位数字序号）" << std::endl;
                }
            }
        }

        if (!success) 
        {
            std::cout << "没有成功重命名任何文件。" << std::endl;
            std::cout << "是否重试？(y/n): ";
            char retry;
            std::cin >> retry;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清空输入缓冲区
            if (retry == 'y' || retry == 'Y') 
            {
                continue; // 重试
            }
            else 
            {
                std::cout << "程序退出。" << std::endl;
                return 1;
            }
        }

        else 
        {
            std::cout << "文件重命名完成！" << std::endl;
            break;
        }
    }

    return 0;
}
