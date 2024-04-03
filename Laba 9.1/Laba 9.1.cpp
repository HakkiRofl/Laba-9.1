#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
using namespace std;

void menu(); // Функция цикличного меню
struct BILET* user_input(BILET* bilet, int* count, ofstream &log); // Функция добавления пассажира пользователем
void read_and_print_bilets(BILET* bilet, int* count, ofstream& log); // Функция чтения пассажиров из памяти
void export_to_file(BILET* bilet, int* count, const char file_name[], ofstream& log); // Экпортируем пассажиров в файл из памяти
struct BILET* import_from_file(BILET* bilet, int* count, const char file_name[], ofstream& log); // Импортируем пассажиров из файла в память
char** getlines(const char file_name[], int* len_text); // Читает файл построчно и каждую строчку записывает в массив
struct BILET* sorting(BILET* bilet, int* count); // Простая пузырьковая сортировка по дате
void bilet_near(BILET* bilet, int* count, ofstream& log); // Поиск близжайшего рейса
void print_passenger_flight(BILET* bilet, int* count, ofstream& log); // Вывод пассажиров указанного рейса
void flight_airport(BILET* bilet, int* count, ofstream& log); // Вывод всех рейсов прилетающих и вылетающих из данного аэропорта
int strlen(char* str); // Функция возвращающая длину строки (дополнение к функции добавления пассажира)


struct airport
{
    char city[36];
    char code[4];
};

struct company
{
    char eng[32];
    char rus[32];
};

struct BILET
{
    char surname1[32]; // Фамилия
    char name[32];
    char surname2[32]; // Отчество
    company company;
    airport departure_airport;
    airport arrival_airport;
    unsigned int flight;
    unsigned int date; // 2023 12 31
    unsigned int time; // 23 59
};

#define airports_count 10
airport airports[] = { {"Томск", "TOF"}, {"Иркутск", "IKT"}, {"Красноярск","KJA"}, {"Нижний Новгород", "GOJ"}, {"Нижневартовск", "NJC"},
    {"Сочи", "AER"}, {"Улан-Удэ", "UUD"}, {"Уфа", "UFA"}, {"Якутск", "YKS"}, {"Южно-Сахалинск", "UUS"}};

#define company_count 5
company companys[] = { {"Aeroflot", "Аэрофлот"}, {"Aurora", "Аврора"}, {"Red Wings", "Ред Вингс"}, {"Rossiya", "Россия"}, {"S7 Airlanes", "Сибирь"} };

int main()
{
    setlocale(LC_ALL, "RUS");
    menu();
}

void menu()
{
    int* count = new int{ 0 }; // Количество билетов в памяти
    struct BILET* bilet = (BILET*)malloc(*count * sizeof(BILET)); // Инициализация массива
    ofstream log("log.txt");
    while (1)
    {  
        cout << "Для продолжения нажмите кнопку нужного действия:\nОтображение списка всех пассажиров - (1)\nДобавление пассажира в список - (2)\n";
        cout << "Экспорт пассажиров в файл (\"database.txt\") - (3)\nИмпорт паcсижров из файла (\"database.txt\") - (4)\nСортировка по времени - (5)\n";
        cout << "Вывод ближайший к заданному времени вылет - (6)\nВывод пассажиров рейса - (7)\nСписок рейсов из указанного аэропорта - (8)\n";
        log << "Для продолжения нажмите кнопку нужного действия:\nОтображение списка всех пассажиров - (1)\nДобавление пассажира в список - (2)\n";
        log << "Экспорт пассажиров в файл (\"database.txt\") - (3)\nИмпорт паcсижров из файла (\"database.txt\") - (4)\nСортировка по времени - (5)\n";
        log << "Вывод ближайший к заданному времени вылет - (6)\nВывод пассажиров рейса - (7)\nСписок рейсов из указанного аэропорта - (8)\n";
        char ch = _getwch();
        switch (ch)
        {
        case 27: cout << "\nНажата клавиша Esc\n" << endl; log << "\nНажата клавиша Esc\n" << endl;
        {
            log.close();
            exit(0);
        }
        case 13: cout << "\nнажата клавиша Enter\n" << endl; log << "\nнажата клавиша Enter\n" << endl; break;
        case 56: cout << "\nНажата клавиша 8\n" << endl; log << "\nНажата клавиша 8\n" << endl;
        {
            flight_airport(bilet, count, log);
            break;
        }
        case 55: cout << "\nНажата клавиша 7\n" << endl; log << "\nНажата клавиша 7\n" << endl;
        {
            print_passenger_flight(bilet, count, log);
            break;
        }
        case 54: cout << "\nНажата клавиша 6\n" << endl; log << "\nНажата клавиша 6\n" << endl;
        {
            bilet_near(bilet, count, log);
            break;
        }
        case 53: cout << "\nНажата клавиша 5\n" << endl; log << "\nНажата клавиша 5\n" << endl;
        {
            bilet = sorting(bilet, count);
            break;
        }
        case 52: cout << "\nНажата клавиша 4\n" << endl; log << "\nНажата клавиша 4\n" << endl;
        {
            bilet = import_from_file(bilet, count, "database.txt", log);
            break;
        }
        case 51: cout << "\nНажата клавиша 3\n" << endl; log << "\nНажата клавиша 3\n" << endl;
        {
            export_to_file(bilet, count, "database.txt", log);
            break;
        }
        case 50: cout << "\nНажата клавиша 2\n" << endl; log << "\nНажата клавиша 2\n" << endl;
        {
            bilet = user_input(bilet, count, log);
            //cout << bilet[0].name << endl;
            //cout << bilet[1].name << endl;

            break;
        }
        case 49: cout << "\nНажата клавиша 1\n" << endl; log << "\nНажата клавиша 1\n" << endl;
        {   
            if (count == 0)
            {
                cout << "Пусто! Заполните список пассажиров!\n\n";
                log << "Пусто! Заполните список пассажиров!\n\n";
            }
            else
                read_and_print_bilets(bilet, count, log); 
        }
        //default: cout << "нажата другая клавиша" << endl; break;
        };

    }
}

struct BILET* user_input(BILET* bilet, int *count, ofstream &log)
{
    //system("cls");
    while (1)
    {
    //ifstream names("passenger.txt", ios_base::in);
    cout << "\nФункция добавления пассажира:\nДля добавления нажмите ENTER, для выхода - ESC\n";
    log << "\nФункция добавления пассажира:\nДля добавления нажмите ENTER, для выхода - ESC\n";
    char ch = _getwch();
    switch (ch)
    {
    case 27: cout << "нажата клавиша ESC - Выход\n" << endl; log << "нажата клавиша ESC - Выход\n" << endl;
    {
        return bilet; 
    }
    case 13: cout << "нажата клавиша Enter\n" << endl; log << "нажата клавиша Enter\n" << endl;
    {
        char surname1[32];
        char name[32];
        char surname2[32];
        short int companys_locale = 0;
        short int departure_airport = 0;
        short int arrival_airport = 0;
        unsigned int flight;
        unsigned int date = 0;
        unsigned int time = 0;

        while (1)
        {
            cout << "\nВведите фамилию пассажира: ";
            log << "\nВведите фамилию пассажира: ";
            cin >> surname1;
            if (strlen(surname1) >= 2)
            {
                break;
            }
            else
            {
                cout << "Фамилия должна содержать минимум 2 символа\n";
                log << "Фамилия должна содержать минимум 2 символа\n";
            }
        }
        while (1)
        {
            cout << "Введите имя пассажира: ";
            log << "Введите имя пассажира: ";
            cin >> name;
            if (strlen(name) >= 2)
            {
                break;
            }
            else
            {
                cout << "\nИмя должно содержать минимум 2 символа\n";
                log << "\nИмя должно содержать минимум 2 символа\n";
            }
        }
        while (1)
        {
            cout << "Введите отчество пассажира: ";
            log << "Введите отчество пассажира: ";
            cin >> surname2;
            if (strlen(surname2) >= 4)
            {
                break;
            }
            else
            {
                cout << "Отчество должно содержать минимум 4 символа\n";
                log << "Отчество должно содержать минимум 4 символа\n";
            }
        }
        cout << "\nИз списка выберите авиакомпанию:\n";
        log << "\nИз списка выберите авиакомпанию:\n";
        for (int i = 0; i < company_count; i++)
        {
            char buffer[100];
            sprintf(buffer, "%i) %s / %s\n", i + 1, companys[i].eng, companys[i].rus);
            cout << buffer;
            log << buffer;
        }
        while (1)
        {
            //int num = 0;
            cout << "Введите номер авиакомании: ";
            log << "Введите номер авиакомании: ";
            cin >> companys_locale;
            //companys_locale = companys[num].eng;
            if (companys_locale > company_count)
            {
                cout << "Введите число меньше " << company_count << endl;
                log << "Введите число меньше " << company_count << endl;
            }
            else
            {
                //strcpy(companys_locale.eng, companys[num - 1].eng);
                //strcpy(companys_locale.rus, companys[num - 1].rus);
                break;
            }
        }
        cout << "\nСписок аэропортов:\n";
        for (int i = 0; i < airports_count; i++)
        {
            char buffer[100];
            sprintf(buffer, "%i) %s (%s)\n", i + 1, airports[i].city, airports[i].code);
            cout << buffer;
            log << buffer;
        }
        while (1)
        {
            //int num = 0;
            cout << "\nИз списка выберите аэропорт вылета:";
            log << "\nИз списка выберите аэропорт вылета:";
            cin >> departure_airport;
            if (departure_airport > airports_count)
            {
                cout << "Введите число меньше " << airports_count << endl;
                log << "Введите число меньше " << airports_count << endl;
            }
            else
            {
                //strcpy(departure_airport, airports[num].code);
                break;
            }
        }
        while (1)
        {
            //int num = 0;
            cout << "\nИз списка выберите аэропорт прилёта:";
            log << "\nИз списка выберите аэропорт прилёта:";
            cin >> arrival_airport;
            if (arrival_airport > airports_count)
            {
                cout << "Введите число меньше " << airports_count << endl;
                log << "Введите число меньше " << airports_count << endl;
            }
            else
            {
                //strcpy(arrival_airport, airports[num].code);
                //if (strcmp(arrival_airport, departure_airport) == 0)
                if (arrival_airport == departure_airport)
                {
                    cout << "Аэропорт прилета точно такой же, как и аэропорт вылета\nПовторите попытку!\n";
                    log << "Аэропорт прилета точно такой же, как и аэропорт вылета\nПовторите попытку!\n";
                }
                else break;
            }
        }
        while (1)
        {
            cout << "\nВведите номер рейса: ";
            log << "\nВведите номер рейса: ";
            cin >> flight;
            if (flight <= 9999 && flight >= 1000)
            {
                break;
            }
            else
            {
                cout << "\nНомер рейста должен быть в формате **** и начинаться с 1000\n";
                log << "\nНомер рейста должен быть в формате **** и начинаться с 1000\n";
            }
        }
        while (1)
        {
            short int year = 0;
            cout << "\nВведите время вылета (год): ";
            log << "\nВведите время вылета (год): ";
            cin >> year;
            if (year <= 2024 && year >= 2022)
            {
                date += year * 10000;
                break;
            }
            else
            {
                cout << "\nВы ввели год раньше 2022 или позже 2024\n";
                log << "\nВы ввели год раньше 2022 или позже 2024\n";
            }
        }
        while (1)
        {
            short int month = 0;
            cout << "\nВведите время вылета (месяц): ";
            log << "\nВведите время вылета (месяц): ";
            cin >> month;
            if (month <= 12 && month >= 1)
            {
                date += month * 100;
                break;
            }
            else
            {
                cout << "\nВы ввели несуществующий месяц\n";
                log << "\nВы ввели несуществующий месяц\n";
            }
        }
        while (1)
        {
            short int day = 0;
            cout << "\nВведите время вылета (день): ";
            log << "\nВведите время вылета (день): ";
            cin >> day;
            if (day <= 31 && day >= 1)
            {
                date += day;
                break;
            }
            else
            {
                cout << "\nВы ввели несуществующий день\n";
                log << "\nВы ввели несуществующий день\n";
            }
        }
        while (1)
        {
            short int hours = 0;
            cout << "\nВведите время вылета (час): ";
            log << "\nВведите время вылета (час): ";
            cin >> hours;
            if (hours <= 23 && hours >= 0)
            {
                time += hours * 100;
                break;
            }
            else
            {
                cout << "\nВы ввели несуществующий час\n";
                log << "\nВы ввели несуществующий час\n";
            }
        }
        while (1)
        {
            short int minutes = 0;
            cout << "\nВведите время вылета (минуты): ";
            log << "\nВведите время вылета (минуты): ";
            cin >> minutes;
            if (minutes <= 59 && minutes >= 0)
            {
                time += minutes;
                break;
            }
            else
            {
                cout << "\nВы ввели несуществующие минуты\n";
                log << "\nВы ввели несуществующие минуты\n";
            }
        }
        char buffer[256];
        sprintf(buffer, "\nПроверьте данные, для сохранения нажмите ENTER, для сброса ЛЮБУЮ другую клавишу\n\nФИО: %s %s %s\nКомпания: %s / %s\n(Рейс %i) %s >>> %s\nДата: %02i-%02i-%i\nВремя: %02i:%02i",
            surname1, name, surname2, companys[companys_locale - 1].eng, companys[companys_locale - 1].rus, flight, airports[departure_airport - 1].city,
            airports[arrival_airport - 1].city, date % 100, (date % 10000) / 100, date / 10000, time / 100, time % 100);
        cout << buffer;
        log << buffer;
        char chr = _getwch();
        switch (chr)
        {
        case 13: cout << "\nНажата клавиша Enter - Сохранение данных\n" << endl; log << "\nНажата клавиша Enter - Сохранение данных\n" << endl;
        {
            *count += 1;
            cout << endl << *count << endl;
            bilet = (BILET*)realloc(bilet, *count * sizeof(BILET));
            strcpy(bilet[*count - 1].surname1, surname1);
            strcpy(bilet[*count - 1].name, name);
            strcpy(bilet[*count - 1].surname2, surname2);
            strcpy(bilet[*count - 1].company.rus, companys[companys_locale - 1].rus);
            strcpy(bilet[*count - 1].company.eng, companys[companys_locale - 1].eng);
            bilet[*count - 1].flight = flight;
            strcpy(bilet[*count - 1].departure_airport.city, airports[departure_airport - 1].city);
            strcpy(bilet[*count - 1].departure_airport.code, airports[departure_airport - 1].code);
            strcpy(bilet[*count - 1].arrival_airport.city, airports[arrival_airport - 1].city);
            strcpy(bilet[*count - 1].arrival_airport.code, airports[arrival_airport - 1].code);
            bilet[*count - 1].date = date;
            bilet[*count - 1].time = time;

            //cout << bilet[0].name;
        }
        }
    }
    };

    }
}

void read_and_print_bilets(BILET *bilet, int* count, ofstream &log)
{
    //system("cls");
    for (int i = 0; i < *count; i++)
    {
        char buffer[256];
        sprintf(buffer, "\n%i)\nФИО: %s %s %s\nКомпания: %s / %s\n(Рейс %i) %s >>> %s\nДата: %02i-%02i-%i\nВремя: %02i:%02i\n", i + 1,
            bilet[i].surname1, bilet[i].name, bilet[i].surname2, bilet[i].company.eng, bilet[i].company.rus, bilet[i].flight, bilet[i].departure_airport.city,
            bilet[i].arrival_airport.city, bilet[i].date % 100, (bilet[i].date % 10000) / 100, bilet[i].date / 10000, bilet[i].time / 100, bilet[i].time % 100);
        cout << buffer;
        log << buffer;
    }
    cout << endl << endl;
    log << endl << endl;
}

void export_to_file(BILET* bilet, int* count, const char file_name[], ofstream& log)
{
    ofstream new_file(file_name);
    for (int i = 0; i < *count; i++)
    {
        char str[256];
        sprintf(str,"%s\n%s\n%s\n%s\n%s\n%i\n%s\n%s\n%i\n%i\n",
            bilet[i].surname1, bilet[i].name, bilet[i].surname2, bilet[i].company.eng, bilet[i].company.rus, bilet[i].flight, bilet[i].departure_airport.city,
            bilet[i].arrival_airport.city, bilet[i].date, bilet[i].time);
        new_file << str;
    }
    new_file.close();
    cout << "\n Успешный экспорт!\n";
    log << "\n Успешный экспорт!\n";
}

struct BILET* import_from_file(BILET* bilet, int* count, const char file_name[], ofstream& log)
{
    int len_text = 0;
    char** strings = getlines(file_name, &len_text);
    cout << endl << len_text << endl;
    //for (int i = 0; i < len_text - 1; i++)
    //{
    //    cout << strings[i] << endl;
    //}

    for (int i = 0; i < (len_text - 1) / 10; i++)
    {
        //cout << i << endl;
        *count += 1;
        bilet = (BILET*)realloc(bilet, *count * sizeof(BILET));
        //cout << atoi(strings[(i * 10) + 5]);
        strcpy(bilet[*count - 1].surname1, strings[(i * 10)]);
        strcpy(bilet[*count - 1].name, strings[(i * 10) + 1]);
        strcpy(bilet[*count - 1].surname2, strings[(i * 10) + 2]);
        strcpy(bilet[*count - 1].company.rus, strings[(i * 10) + 3]);
        strcpy(bilet[*count - 1].company.eng, strings[(i * 10) + 4]);
        bilet[*count - 1].flight =  atoi(strings[(i * 10) + 5]);
        strcpy(bilet[*count - 1].departure_airport.city, strings[(i * 10) + 6]);
        strcpy(bilet[*count - 1].arrival_airport.city, strings[(i * 10) + 7]);
        bilet[*count - 1].date = atoi(strings[(i * 10) + 8]);
        bilet[*count - 1].time = atoi(strings[(i * 10) + 9]);
    }
    cout << "\nУспешный импорт!";
    log << "\nУспешный импорт!";
    return bilet;
}


char** getlines(const char file_name[], int* len_text)
{
    ifstream file(file_name, ios_base::in);
    if (file.is_open())
    {
        int* count_arrays = new int{ 1 };// Длина массива указателей на строку (Создали #1)
        char** array_of_arrays;
        array_of_arrays = (char**)malloc(*count_arrays * sizeof(char*)); // Массив указателей (Создали #1)

        int* len_array = new int{ 1 }; // Длина строки символов (Создали #3)
        char* array = (char*)malloc(*len_array * sizeof(char)); // Массив символов строки (Создали #4)

        char letter; // Статическая переменная хранящая символ строки

        fstream file(file_name, fstream::in);

        while (file >> noskipws >> letter)
        {
            if (letter != '\n')
            {
                //cout << letter;
                *len_array += 1;
                array = (char*)realloc(array, *len_array * sizeof(char)); // Пересоздание массива символов с увеличением кол-ва символов на 1
                array[*len_array - 2] = letter; // Запись в массив прочитанного символа
            }
            else
            {
                array[*len_array - 1] = '\0';
                array_of_arrays[*count_arrays - 1] = array;
                *len_array = 1;
                *count_arrays += 1;
                array_of_arrays = (char**)realloc(array_of_arrays, *count_arrays * sizeof(char*));
                array = (char*)malloc(*len_array * sizeof(char));
            }
        }
        array[*len_array - 1] = '\0';
        array_of_arrays[*count_arrays] = array;
        *len_text = *count_arrays;
        delete count_arrays, len_array; // (Удалили #1, #3)
        file.close(); // Закрытие файла
        return array_of_arrays;
    }
    else
    {
        cout << "Не удалось открыть файл!\n";
        exit;
    }
}


struct BILET* sorting(BILET* bilet, int* count)
{
    for (int i = 0; i < *count; i++)
    {
        for (int k = 0; k < *count - 1; k++)
        {
            if ((bilet[k].date * 1000 + bilet[k].time) > (bilet[k + 1].date * 1000 + bilet[k + 1].time))
            {
                swap(bilet[k], bilet[k + 1]);
            }
        }
    }
    return bilet;
}

void bilet_near(BILET* bilet, int* count, ofstream &log)
{
    //system("cls");
    cout << "Для поиска близжайшего рейса введите дату и время:\n\n";
    log << "Для поиска близжайшего рейса введите дату и время:\n\n";
    //read_and_print_bilets(bilet, count);
    unsigned long long date = 0;
    unsigned long long time = 0;
    while (1)
    {
        short int year = 0;
        cout << "\nВведите время вылета (год): ";
        log << "\nВведите время вылета (год): ";
        cin >> year;
        if (year <= 2024 && year >= 2022)
        {
            date += year * 10000;
            break;
        }
        else
        {
            cout << "\nВы ввели год раньше 2022 или позже 2024\n";
            log << "\nВы ввели год раньше 2022 или позже 2024\n";
        }
    }
    while (1)
    {
        short int month = 0;
        cout << "\nВведите время вылета (месяц): ";
        log << "\nВведите время вылета (месяц): ";
        cin >> month;
        if (month <= 12 && month >= 1)
        {
            date += month * 100;
            break;
        }
        else
        {
            cout << "\nВы ввели несуществующий месяц\n";
            log << "\nВы ввели несуществующий месяц\n";
        }
    }
    while (1)
    {
        short int day = 0;
        cout << "\nВведите время вылета (день): ";
        log << "\nВведите время вылета (день): ";
        cin >> day;
        if (day <= 31 && day >= 1)
        {
            date += day;
            break;
        }
        else
        {
            cout << "\nВы ввели несуществующий день\n";
            log << "\nВы ввели несуществующий день\n";
        }
    }
    while (1)
    {
        short int hours = 0;
        cout << "\nВведите время вылета (час): ";
        log << "\nВведите время вылета (час): ";
        cin >> hours;
        if (hours <= 23 && hours >= 0)
        {
            time += hours * 100;
            break;
        }
        else
        {
            cout << "\nВы ввели несуществующий час\n";
            log << "\nВы ввели несуществующий час\n";
        }
    }
    while (1)
    {
        short int minutes = 0;
        cout << "\nВведите время вылета (минуты): ";
        log << "\nВведите время вылета (минуты): ";
        cin >> minutes;
        if (minutes <= 59 && minutes >= 0)
        {
            time += minutes;
            break;
        }
        else
        {
            cout << "\nВы ввели несуществующие минуты\n";
            log << "\nВы ввели несуществующие минуты\n";
        }
    }
    unsigned int result = bilet[0].date;
    long long num_flight = 0;
    cout << endl << (bilet[0].date) * 10e3 + bilet[0].time << endl;
    for (int i = 0; i < *count; i++)
    {
        long long buffer = (long long)(bilet[i].date * 10e3 + bilet[i].time);
        long long result_buffer = (date * 10e3 + time) - buffer;
        if (result_buffer < 0)
        {
            result_buffer *= -1;
        }
        if (result_buffer < result)
        {
            result = result_buffer;
            num_flight = i;
        }
        //printf("result:%i\nbuffer:%i\nresult_buffer:%i\n");
    }
    char buffer[256];
    sprintf(buffer, "\n%i)\nФИО: %s %s %s\nКомпания: %s / %s\n(Рейс %i) %s >>> %s\nДата: %02i-%02i-%i\nВремя: %02i:%02i\n", num_flight + 1,
        bilet[num_flight].surname1, bilet[num_flight].name, bilet[num_flight].surname2, bilet[num_flight].company.eng, bilet[num_flight].company.rus, bilet[num_flight].flight, bilet[num_flight].departure_airport.city,
        bilet[num_flight].arrival_airport.city, bilet[num_flight].date % 100, (bilet[num_flight].date % 10000) / 100, bilet[num_flight].date / 10000, bilet[num_flight].time / 100, bilet[num_flight].time % 100);
    cout << buffer;
    log << buffer;
    /*
    for (int i = 0; i < *count; i++)
    {
        if (date = bilet[i].date)
        {
            if (time - bilet[i].time <= 60 and time - bilet[i].time >= -60)
            {
                printf("\n%i)\nФИО: %s %s %s\nКомпания: %s / %s\n(Рейс %i) %s >>> %s\nДата: %02i-%02i-%i\nВремя: %02i:%02i\n", i + 1,
                    bilet[i].surname1, bilet[i].name, bilet[i].surname2, bilet[i].company.eng, bilet[i].company.rus, bilet[i].flight, bilet[i].departure_airport.city,
                    bilet[i].arrival_airport.city, bilet[i].date % 100, (bilet[i].date % 10000) / 100, bilet[i].date / 10000, bilet[i].time / 100, bilet[i].time % 100);
            }
            else cout << "\nВ ближайшее время нет вылетов и прилетов";
        }
        else cout << "\nВ этот день нет рейсов\n";
    }
    */
}

void print_passenger_flight(BILET* bilet, int* count, ofstream &log)
{
    cout << "\nВывод пассажиров рейса";
    log << "\nВывод пассажиров рейса";
    int num_flight = 0;
    cout << "\nВыберите номер рейса из списка:\n";
    log << "\nВыберите номер рейса из списка:\n";
    for (int i = 0; i < *count; i++)
    {
        cout << bilet[i].flight;
        log << bilet[i].flight;
    }
    cout << "\nРейс: ";
    log << "\nРейс: ";
    cin >> num_flight;
    for (int i = 0; i < *count; i++)
    {
        if (num_flight == bilet[i].flight)
        {
            //cout << num_flight;
            char buffer[256];
            sprintf(buffer, "\n%i)\nФИО: %s %s %s\nКомпания: %s / %s\n(Рейс %i) %s >>> %s\nДата: %02i-%02i-%i\nВремя: %02i:%02i\n", i + 1,
                bilet[i].surname1, bilet[i].name, bilet[i].surname2, bilet[i].company.eng, bilet[i].company.rus, bilet[i].flight, bilet[i].departure_airport.city,
                bilet[i].arrival_airport.city, bilet[i].date % 100, (bilet[i].date % 10000) / 100, bilet[i].date / 10000, bilet[i].time / 100, bilet[i].time % 100);
            cout << buffer;
            log << buffer;
        }
    }
    cout << endl;
    log << endl;
}


void flight_airport(BILET* bilet, int* count, ofstream &log)
{
    short int num = 0;
    cout << "\nВсе рейсы с заданного аэропорта\n";
    log << "\nВсе рейсы с заданного аэропорта\n";
    cout << "\nСписок аэропортов:\n";
    log << "\nСписок аэропортов:\n";
    for (int i = 0; i < airports_count; i++)
    {
        char buffer[100];
        sprintf(buffer, "%i) %s (%s)\n", i + 1, airports[i].city, airports[i].code);
        cout << buffer;
        log << buffer;
    } 
    while (1)
    {
        cout << "\nВыберите номер аэропорта:";
        log << "\nВыберите номер аэропорта:";
        cin >> num;
        if (num > airports_count)
        {
            cout << "Введите число меньше " << airports_count << endl;
            log << "Введите число меньше " << airports_count << endl;
        }
        else
        {
            break;
        }
    }
    cout << "\nВылетают из аэропорта рейсы:\n";
    log << "\nВылетают из аэропорта рейсы:\n";
    for (int i = 0; i < *count; i++)
    {
        if (!strcmp(airports[num - 1].city, bilet[i].departure_airport.city))
        {
            cout << bilet[i].flight << endl;
            log << bilet[i].flight << endl;
        }      
    }   
    cout << "\nПрилетают в аэропорт рейсы:\n";
    log << "\nПрилетают в аэропорт рейсы:\n";
    for (int i = 0; i < *count; i++)
    {
        if (!strcmp(airports[num - 1].city, bilet[i].arrival_airport.city))
        {
            cout << bilet[i].flight << endl;
            log << bilet[i].flight << endl;
        }
    }
}


int strlen(char* str)
{
    int count = 0;
    while (str[count] != '\0') count++;
    return count;
}


/*
void generation(const char *file_names, const char *file_surnames, const char *file_airports, int count)
{
    ofstream new_file("passenger.txt");
    ifstream names(file_names, ios_base::in);
    ifstream surnames(file_surnames, ios_base::in);
    ifstream airports(file_airports, ios_base::in);
    for (int i = 0; i < count; i++)
    {
     
    }

}
*/