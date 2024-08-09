#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>
#include <Windows.h>
#include <algorithm>
#include <codecvt>
#include <locale>
#include <iomanip>
#include <map>
#include <unordered_map>
#include <set>


class Object
{
public:
    std::wstring Name;
    double X;
    double Y;
    std::wstring Type;
    double Time;
};

int Output(int p)
{
    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::wcout.imbue(std::locale());
    std::wcout << L"Чтобы сгруппировать по дистанции, отправьте 1 " << std::endl;
    std::wcout << L"Чтобы сгруппировать по имени, отправьте 2" << std::endl;
    std::wcout << L"Чтобы сгруппировать по времени создания, отправьте 3" << std::endl;
    std::wcout << L"Чтобы сгруппировать по типу, отправьте 4" << std::endl;
    std::wcout << L"Чтобы закрыть программу, отправьте 5" << std::endl;
    std::cin >> p;
    return p;
}


std::vector <Object> ReadFile(const std::string& File)
{
    std::vector <Object> Objects;
    std::wifstream file(File, std::ios::in | std::ios::binary);
    while (file.is_open())
    {
        file.imbue(std::locale("ru_RU.UTF-8"));
        std::wstring Line;
        while (std::getline(file, Line)) {
            Object object;
            std::wistringstream iss(Line);
            iss >> object.Name >> object.X >> object.Y >> object.Type >> object.Time;
            Objects.push_back(object);
        }
        file.close();
    }
    return Objects;
}


double Distance(const Object& object)
{
    return sqrt(pow(object.X, 2) + pow(object.Y, 2));
}


std::vector<std::vector<Object>> GroupbyDistance(const std::vector<Object>& objects)
{
    std::vector<std::vector<Object>> groups(4);
    for (const auto& obj : objects)
    {
        double distance = Distance(obj);
        if (distance <= 100)
            groups[0].push_back(obj);
        else if (distance <= 1000)
            groups[1].push_back(obj);
        else if (distance <= 10000)
            groups[2].push_back(obj);
        else
            groups[3].push_back(obj);
    }

    for (auto& group : groups)
    {
        std::sort(group.begin(), group.end(), [](const Object& obj1, const Object& obj2)
        {
            return Distance(obj1) < Distance(obj2);
        });
    }

    return groups;
}


void OutputGroupbyDistance(const std::vector<std::vector<Object>>& groups, const std::string& File)
{
    std::wofstream file(File);
    file.imbue(std::locale("ru_RU.UTF-8"));

    if (file.is_open())
    {
        for (size_t i = 0; i < groups.size(); i++)
        {
            std::wstring GroupName;
            switch (i)
            {
            case 0: GroupName = L"Группа от 0 до 100 Метров"; break;
            case 1: GroupName = L"Группа от 100 до 1000 Метров"; break;
            case 2: GroupName = L"Группа от 1000 до 10000 Метров"; break;
            case 3: GroupName = L"Другие"; break;
            }
            file << GroupName << std::endl;
            if (groups[i].empty())
                file << L"Группа не содержит элементов" << std::endl<<std::endl;
            else
            {
                for (const auto& obj : groups[i])
                    file << std::fixed << std::setprecision(3) << obj.Name << " " << obj.X << " " << obj.Y << " " << obj.Type << " " << obj.Time << std::endl;
                file<< std::endl;
            }
                
        }
        file.close();
    }
}


std::vector<std::vector<Object>> GroupbyName(const std::vector<Object>& objects)
{

    std::vector<std::vector<Object>> groups(32);
    for (const auto& obj : objects)
    {
        wchar_t FirstLetter = obj.Name[0];

        int Index = FirstLetter - L'А';

        if (FirstLetter == L'Ё')
            groups[6].push_back(obj);
        else if (iswupper(FirstLetter) && FirstLetter >= L'А' && FirstLetter <= L'Я')
        {
            if (Index < 6)
                groups[FirstLetter - L'А'].push_back(obj);
            else
                groups[FirstLetter - L'А' + 1].push_back(obj);
        }
        else
            groups[31].push_back(obj);
    }


    for (auto& group : groups) {
        std::sort(group.begin(), group.end(), [](const Object& obj1, const Object& obj2) {
            return obj1.Name < obj2.Name;
            });
    }


    return groups;
}


void OutputGroupbyName(const std::vector<std::vector<Object>>& groups, const std::string& File)
{
    std::wofstream file(File);
    file.imbue(std::locale("ru_RU.UTF-8"));

    const wchar_t* alphabet = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЫЭЮЯ#";

    if(file.is_open())
    {
        for (size_t i = 0; i < groups.size(); i++)
        {
            if (groups[i].empty())
            {
                file << L"Группа " << alphabet[i] << std::endl;
                file << L"Группа не содержит элементов" << std::endl<<std::endl;
            }
            else
            {
                file << L"Группа " << alphabet[i] << std::endl;
                for (const auto& obj : groups[i])
                    file << std::fixed << std::setprecision(3) << obj.Name << " " << obj.X << " " << obj.Y << " " << obj.Type << " " << obj.Time << std::endl;
                file <<std::endl;
            }    
        }
        file.close();
    }   
}


std::vector<std::vector<Object>> GroupbyTime(const std::vector<Object>& objects)
{
    std::vector<std::vector<Object>> groups(6);
    for (auto& obj : objects)
    {
        if (obj.Time <= 86400) 
            groups[0].push_back(obj);
        else if (obj.Time <= 172800) 
            groups[1].push_back(obj);
        else if (obj.Time <= 604800)  
            groups[2].push_back(obj); 
        else if (obj.Time <= 2592000) 
            groups[3].push_back(obj);
        else if (obj.Time < 31536000) 
            groups[4].push_back(obj);
        else 
            groups[5].push_back(obj);
    }

    for (auto& group : groups)
    {
        std::sort(group.begin(), group.end(), [](const Object& obj1, const Object& obj2) {
            return obj1.Time < obj2.Time;
            });
    }
    return groups;

}


void OutputGroupbyTime(const std::vector<std::vector<Object>>& groups, const std::string& File)
{
    std::wofstream file(File);
    file.imbue(std::locale("ru_RU.UTF-8"));

    if (file.is_open())
    {
        for (size_t i = 0; i < groups.size(); i++)
        {
            std::wstring GroupName;
            switch (i)
            {
            case 0: GroupName = L"Сегодня"; break;
            case 1: GroupName = L"Вчера"; break;
            case 2: GroupName = L"На этой неделе"; break;
            case 3: GroupName = L"В этом месяце"; break;
            case 4: GroupName = L"В этом году"; break;
            case 5: GroupName = L"Ранее"; break;
            }
            file << GroupName << std::endl;
            if (groups[i].empty())
                file << L"Группа не содержит элементов" << std::endl<<std::endl;
            else
            {
                for (const auto& obj : groups[i])
                    file << std::fixed << std::setprecision(3) << obj.Name << " " << obj.X << " " << obj.Y << " " << obj.Type << " " << obj.Time << std::endl;
                file << std::endl;
            }
                
        }
        file.close();
    }
}


std::vector<std::vector<Object>> GroupbyType(const std::vector<Object>& objects, size_t minGroupSize, const std::string& File)
{
    std::wofstream file(File);
    file.imbue(std::locale("ru_RU.UTF-8"));

    std::vector<std::vector<Object>> groups;
    std::unordered_map<std::wstring, std::vector<Object>> typeGroups;
    std::vector<Object> remainingGroup;

    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::wcout.imbue(std::locale());

    for (const auto& obj : objects)
        typeGroups[obj.Type].push_back(obj);


    for (auto& [type, group] : typeGroups)
        if (group.size() < minGroupSize)
            remainingGroup.insert(remainingGroup.end(), group.begin(), group.end());


    if (!remainingGroup.empty())
        std::sort(remainingGroup.begin(), remainingGroup.end(), [](const Object& obj1, const Object& obj2) {
            return obj1.Name < obj2.Name;
            });


    for (auto& [type, group] : typeGroups)
        if (group.size() >= minGroupSize)
        {
            std::sort(group.begin(), group.end(), [](const Object& obj1, const Object& obj2) {
                return obj1.Name < obj2.Name;
                });
            groups.push_back(std::move(group));
        }

    if (file.is_open())
    {
        for (size_t i = 0; i < groups.size(); i++)
        {
            file << L"Группа " << groups[i].front().Type << std::endl;
            for (const auto& obj : groups[i])
                file << std::fixed << std::setprecision(3) << obj.Name << " " << obj.X << " " << obj.Y << " " << obj.Type << " " << obj.Time << std::endl;
            file << std::endl;
        }

        if (!remainingGroup.empty())
        {
            file << L"Группа прочее" << std::endl;
            for (auto& obj : remainingGroup)
                file << std::fixed << std::setprecision(3) << obj.Name << " " << obj.X << " " << obj.Y << " " << obj.Type << " " << obj.Time << std::endl;
            file << std::endl;
        }
        file.close();
    }
    

    if (!remainingGroup.empty())
        groups.emplace_back(std::move(remainingGroup));


   

    return groups;
}



int main()
{   
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    std::wcout.imbue(std::locale(""));
    int p = 0;

    std::vector <Object> objects = ReadFile("Input.txt");

    while (p != 5)
    {
        p = Output(p);

        if (p == 1)
        {
            OutputGroupbyDistance(GroupbyDistance(objects), "Output.txt");
            std::wcout << L"Проверьте Output.txt" << std::endl << std::endl;
        }
        if (p == 2)
        {
            OutputGroupbyName(GroupbyName(objects), "Output.txt");
            std::wcout << L"Проверьте Output.txt" << std::endl << std::endl;
        }
        if (p == 3)
        {
            OutputGroupbyTime(GroupbyTime(objects), "Output.txt");
            std::wcout << L"Проверьте Output.txt" << std::endl << std::endl;
        }
        if (p == 4)
        {
            int n = 0;
            std::wcout << L"Укажите, какое минимальное количество объектов нужно для создания группы " << std::endl;
            std::cin >> n;
            std::vector<std::vector<Object>> groupbytype = GroupbyType(objects, n, "Output.txt");
            std::wcout << L"Проверьте Output.txt" << std::endl << std::endl;
        }
    }
    

}

