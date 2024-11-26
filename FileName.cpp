#include <iostream>
#include <filesystem>
#include <regex>
#include <string>

namespace fs = std::filesystem;

// �Ƴ�·���е�˫���ţ�����У�
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
        // �û������Ŀ¼·��
        std::string directory;
        std::cout << "�������ļ�����Ŀ¼·��: ";
        std::getline(std::cin, directory);
        directory = clean_path(directory);

        // ���Ŀ¼�Ƿ����
        if (!fs::exists(directory) || !fs::is_directory(directory)) 
        {
            std::cout << "����: ·����Ч����һ���ļ��У�" << std::endl;
            std::cout << "�Ƿ����ԣ�(y/n): ";
            char retry;
            std::cin >> retry;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������뻺����
            if (retry == 'y' || retry == 'Y')
            {
                continue; // ����
            }
            else 
            {
                std::cout << "�����˳���" << std::endl;
                return 1;
            }
        }

        // �û�������Զ�������
        std::string custom_text;
        std::cout << "��������Ҫ���Զ�������: ";
        std::getline(std::cin, custom_text);

        // ƥ���ļ�����������ʽ��������λ���ֵ����
        std::regex pattern(R"(.*?(\d{2})(.*?)(\.[^.]+)?)");

        // ����Ŀ¼�е��ļ�
        bool success = false;
        for (const auto& entry : fs::directory_iterator(directory)) 
        {
            if (entry.is_regular_file()) 
            {
                std::string filename = entry.path().filename().string();
                std::smatch match;

                // ƥ���ļ����е���λ�������
                if (std::regex_match(filename, match, pattern)) 
                {
                    std::string number = match[1];    // ��ȡ��λ����
                    std::string extension = match[3].str(); // ��ȡ��չ����������ڣ�

                    // �����µ��ļ������Զ������� + �»��� + ��λ���� + ��չ��
                    std::string new_name = custom_text + "_" + number + extension;

                    // �������ļ�
                    try
                    {
                        fs::rename(entry.path(), entry.path().parent_path() / new_name);
                        std::cout << "������: " << filename << " -> " << new_name << std::endl;
                        success = true;
                    }
                    catch (const std::exception& e) 
                    {
                        std::cerr << "����: �޷��������ļ� " << filename << "��ԭ��: " << e.what() << std::endl;
                    }
                }
                else
                {
                    std::cout << "����: " << filename << "��δ�ҵ���λ������ţ�" << std::endl;
                }
            }
        }

        if (!success) 
        {
            std::cout << "û�гɹ��������κ��ļ���" << std::endl;
            std::cout << "�Ƿ����ԣ�(y/n): ";
            char retry;
            std::cin >> retry;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������뻺����
            if (retry == 'y' || retry == 'Y') 
            {
                continue; // ����
            }
            else 
            {
                std::cout << "�����˳���" << std::endl;
                return 1;
            }
        }

        else 
        {
            std::cout << "�ļ���������ɣ�" << std::endl;
            break;
        }
    }

    return 0;
}
