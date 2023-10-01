#include<iostream>
#include<cmath>

#include<string>

using namespace std;

class Hamming

{

    string message;
    string allMessage;
    int codeword[50], temp[50];
    string templ;
    int n, check;
    char parity;
    string endWord;
    string endCorrectWord;
    

public:

    Hamming()
    {
        parity = 'E';
        message = "";
        n = check = 0;
        for (int i = 0; i < 50; i++)
        {
            temp[i] = codeword[i] = 0;
        }
        
    }

    string getEndWord() {
        return endWord;
    }

    void generate()
    {
        
        do
        {
            cout << "Введите бинарное сообщение: ";
            cin >> allMessage;
        } while (allMessage.find_first_not_of("01") != string::npos);
        int count = 0;
        for (unsigned int i = 0; i < (allMessage.size() / 4); i++)
        {
            templ = "";
            for (int j = 0; j < 4; j++)
            {
                templ += allMessage[count];
                count++;
            }
            message = templ;
            Hum74();
        }
    }


    void Hum74() {
        n = message.size();
        for (unsigned int i = 0; i < message.size(); i++)
        {
            if (message[i] == '1')
                temp[i + 1] = 1;
            else
                temp[i + 1] = 0;
        }
        computeCode();
    }


    void computeCode()
    {
        check = findr();
        for (int i = (n + check), j = n; i > 0; i--)
        {
            if ((i & (i - 1)) != 0)
                codeword[i] = temp[j--];
            else
                codeword[i] = setParity(i);
        }
        /*cout << "Биты четности - ";
        for (int i = 0; i < check; i++)
            cout << "P" << pow(2, i) << " : " << codeword[(int)pow(2, i)] << "\t";
        cout << endl;*/
        //cout << "Кодовое слово:" << endl;
        for (int i = 1; i <= (n + check); i++) {
            endWord += to_string(codeword[i]);
           // cout << codeword[i] << " ";
        }
        //cout << endl;
    }
    int findr()
    {
        for (int i = 1;; i++)
        {
            if (n + i + 1 <= pow(2, i))
                return i;
        }
    }
    int setParity(int x)
    {
        bool flag = true;
        int bit;
        if (x == 1)
        {
            bit = codeword[x + 2];
            for (int j = x + 3; j <= (n + check); j++)
            {
                if (j % 2)
                {
                    bit ^= codeword[j];
                }
            }
        }
        else
        {
            bit = codeword[x + 1];
            for (int i = x; i <= (n + check); i++)
            {
                if (flag)
                {
                    if (i == x || i == x + 1)
                        bit = codeword[x + 1];
                    else
                        bit ^= codeword[i];
                }
                if ((i + 1) % x == 0)
                    flag = !flag;
            }
        }
        if (parity == 'O' || parity == 'o')
            return !bit;
        else
            return bit;
    }
    void correct()
    {
        do
        {
            cout << "Введите полученное кодовое слово :";
            cin >> allMessage;
        } while (allMessage.find_first_not_of("01") != string::npos);

        int count =  0;
        endWord = "";
        for (unsigned int i = 0; i < (allMessage.size() / 7); i++)
        {
            templ = "";
            for (int j = 0; j < 7; j++)
            {
                templ += allMessage[count];
                count++;
            }
            message = templ;
            correctHum74();
        }
    }

    void correctHum74() {

        for (unsigned int i = 0; i < message.size(); i++)
        {
            if (message[i] == '1')
                codeword[i + 1] = 1;
            else
                codeword[i + 1] = 0;
        }
        detect();
    }

    void detect()
    {
        int position = 0;

        //cout << "Биты четности - ";
        for (int i = 0; i < check; i++)
        {
            bool flag = true;
            int x = pow(2, i);
            int bit = codeword[x];
            if (x == 1)
            {
                for (int j = x + 1; j <= (n + check); j++)
                {
                    if (j % 2)
                    {
                        bit ^= codeword[j];
                    }
                }
            }
            else
            {
                for (int k = x + 1; k <= (n + check); k++)
                {
                    if (flag)
                    {
                        bit ^= codeword[k];
                    }
                    if ((k + 1) % x == 0)
                        flag = !flag;
                }
            }
            //cout << "P" << x << ": " << bit << "\t";
            if ((parity == 'E' || parity == 'e') && bit == 1)
                position += x;
            if ((parity == 'O' || parity == 'o') && bit == 0)
                position += x;
        }
        cout << endl << "Полученное кодовое слово:";
        for (int i = 1; i <= (n + check); i++)
            cout << codeword[i] << " ";
        cout << endl;
        if (position != 0)
        {
            cout << "Ошибка в бите : " << position << endl;
            codeword[position] = !codeword[position];
            cout << "Исправленное кодовое слово :" << endl;
            for (int i = 1; i <= (n + check); i++)
                cout << codeword[i] << " ";
            cout << endl;
        }
        else
            cout << "Ошибки в полученном коде нет." << endl;
        cout << "Полученное сообщение является :";
        for (int i = 1; i <= (n + check); i++)
            if ((i & (i - 1)) != 0) {
                endWord += to_string(codeword[i]);
                cout << codeword[i] << " ";
            }
        cout << endl;
    }
};
int main()
{
    setlocale(LC_ALL, "Russian");

    char choice;
    do
    {
        Hamming a;
        cout << "На стороне отправителя : " << endl;
        a.generate();
        cout << endl <<"Закодированное слово: " <<  a.getEndWord() << endl;

        cout << endl << "На стороне получателя :" << endl;
        a.correct();

        cout << endl << "Разкодированное слово: " << a.getEndWord();
        cout << endl << "Ввести другой код? (Y/N) : ";
        cin >> choice;
        cout << endl;
    } while (choice == 'y' || choice == 'Y');
    return 0;
}
