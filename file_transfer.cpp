#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
/*

Реализовано и протестировано для Mac Os

*/
using namespace std;
using std::filesystem::directory_iterator;

//объявляю прототипы

void readf(string &, string &);
void runp(string &, string &);
void runtest(string &, string &);

string check(string);
bool selection(string);
bool Myselection(string);
void showl(string);

void exzamine(string &, vector<string> &);
void removing(vector<string> &);
void rightFiles(string &, vector<string> &);

string change(string);

void changeGemToHtml(vector<string> &, vector<string> &);

void PreparAndWorkOnFiles(string &, string &, vector<string> &, vector<string> &);

void run();

void GmiToHtml(vector<string> &);

int main()
{
    run();
    return 0;
}

void run()
{
    string input, output; //переменные для пути к каталогам ввода и там, где будут сделаны преобразованя
    runp(input, output);  //функция, которая считывает путь к инпут и аутпут каталогам и делает копию этих файлов в каталог аутпут

    // input = "/Users/artemgudzenko/Desktop/С++/input/";
    // output = "/Users/artemgudzenko/Desktop/С++/output/";
    // runtest(input, output);

    string path = input;   //храненеие исходных файлов
    string pathO = output; //храненеие файлов на изменение

    vector<string> files(1000);  //допустим максимальное число файлов исзодных 1000 (внутри вектора будет храниться полный путь и расширение файлов в каталоге)
    vector<string> filesO(1000); // уже внутри ф-ии делаю resize под размер файлов

    PreparAndWorkOnFiles(path, pathO, files, filesO); //считывание исходных файлов с расширением .gmi и перезапись выходных файлов

    GmiToHtml(filesO); //конвертируем расширение файлов с .gmi в .html
}

void runp(string &stri, string &stro)
{
    readf(stri, stro);
    system(("cp -a " + stri + "/ " + stro + "/").c_str()); //делаю копию файлов через терминал
}

void readf(string &stri, string &stro) //для того, чтобы в консольный файл можно было вводить
{
    cout << "Введите пусть к исходной папке\n=> ";
    cin >> stri;

    cout << "Введите путь для формирования генератора\n=> ";
    cin >> stro;
}

void runtest(string &stri, string &stro) //не играет роли было написано для тестов
{
    system(("cp -a " + stri + " " + stro).c_str());
}

void PreparAndWorkOnFiles(string &path, string &pathO, vector<string> &files, vector<string> &filesO)
{
    exzamine(path, files);   //заполнение вектора инпут файловымы маршрутами
    exzamine(pathO, filesO); //заполнение вектора аутпут файловымы маршрутами
    removing(files);         //удаление из вектора тех файловых путей, у которых нет расширения .gmi
    removing(filesO);
    rightFiles(pathO, filesO);      //вывод количества файлов на изменение и их пути
    changeGemToHtml(files, filesO); //замена содержания файлов .gmi аутпут директории с помощью ф-ии change
}

void exzamine(string &path, vector<string> &files)
{
    int count = 0;
    for (const auto &file : directory_iterator(path))
    {
        files[count] = file.path();
        count++;
    }
    files.resize(count);
    for (int i = 0; i < files.size(); i++)
    {
        cout << files[i] << endl;
    }
}

void removing(vector<string> &str)
{
    auto it = remove_if(str.begin(), str.end(), Myselection); //удаление из вектора маршрутов тех, которые имеют расширения не .gmi
    str.erase(it, str.end());
}
string check(string str) //проверка расширения
{
    int len = str.size();
    string res;
    for (int i = 4; i > 0; i--)
    {
        res += str[len - i];
    }
    return res;
}

bool selection(string str)
{
    if (check(str) != "html")
        return 1;
    else
        return 0;
}

bool Myselection(string str) //нужное нам расширение для удаления из вектора
{
    if (check(str) != ".gmi")
        return 1;
    else
        return 0;
}

void showl(string str)
{
    // cout <<check(str)<<" ";
    if (check(str) == "html")
        cout << "finded";
}

void rightFiles(string &path, vector<string> &files)
{
    cout << "нужные файлы" << endl;

    for (const string &path : files)
    {
        cout << path << endl;
    }
    int col = files.size();
    cout << "Количесвто файлов на изменение: " << col << endl;
}

void changeGemToHtml(vector<string> &vec, vector<string> &vecO)
{
    int count = vec.size();

    while (count)
    {
        cout << "Открытие файла и его редактирование" << endl;

        ifstream file(vec[count - 1]);   //на чтение (исходный каталог)
        ofstream mfile(vecO[count - 1]); //на запись (выходной каталог)

        string str;
        while (getline(file, str)) //получение строки и ее изменение
        {
            str = change(str);    //обработка строки
            mfile << str << endl; // запись в аутпут файл
        }

        file.close();
        count--;
    }
}

string change(string str) //анализ строки файла .gmi
{
    string y = str;

    if (y[0] == '#' && y[1] != '#')
    {
        string tmp = str.substr(1, str.length() - 1);
        str = "<h1> " + tmp + " </h1> ";
    }

    if (y[0] == '#' && y[1] == '#' && y[2] != '#')
    {
        string tmp = str.substr(2, str.length() - 1);
        str = "<h2> " + tmp + " </h2> ";
    }

    if (y[0] == '#' && y[1] == '#' && y[2] == '#')
    {
        string tmp = str.substr(3, str.length() - 1);
        str = "<h3> " + tmp + " </h3> ";
    }

    if (y[0] != '#' && y[0] != '*' && y[0] != '>' && !((y[0] == '=') && (y[1] == '>')) && !((y[0] == '`') && (y[1] == '`') && (y[2] == '`')))
    {
        string tmp = str.substr(0, str.length() - 1);
        str = "<p>" + tmp + "</p>";
    }

    if (y[0] == '*')
    {
        string tmp = str.substr(1, str.length() - 1);
        str = "<li> " + tmp + "</li>";
    }

    if (y[0] == '>')
    {
        string tmp = str.substr(1, str.length() - 1);
        str = "<q> " + tmp + "</q>";
    }

    if (y[0] == '=' && y[1] == '>')
    {

        string y = str;
        int pos = str.find(' ', 4);
        string tmp = y.substr(3, pos - 2);
        int find = str.rfind(' ');
        string tmp2 = str.substr(pos, str.length() - 1);
        string a1 = "<a href=\"";
        str = a1 + tmp + "\">" + tmp2 + "</a>";
    }

    return str;
}

void GmiToHtml(vector<string> &filesO) //замена расширения с .gmi в .html
{
    int count = filesO.size();

    while (count)
    {
        string str = filesO[count - 1];
        int pos = 0;
        string tmp = str.substr(pos, str.size() - 4);
        tmp += ".html";
        cout << tmp << " ";
        count--;
        system(("mv " + str + " " + tmp).c_str());
    }
}
