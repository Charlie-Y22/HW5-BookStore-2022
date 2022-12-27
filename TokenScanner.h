#ifndef INC_1_6BOOKSTORE_TOKENSCANNER_H
#define INC_1_6BOOKSTORE_TOKENSCANNER_H

#include <iostream>
#include <string>
#include <cstring>
//#include "AccountSystem.h"
#include "LogSystem.h"
#include "check.h"

class TokenScanner {
public:
    std::string tokens;
    bool hasMore = true;
    int point = 0;

    TokenScanner() = default;

    explicit TokenScanner(const std::string &s) {
        tokens = s;
    }

    std::string nextToken() {
        while (tokens[point] == ' ' and point != tokens.length()) ++point;
        if (point == tokens.length()) return "";
        int begin = point;
        int end = point;
        while (tokens[end] != ' ' and end != tokens.length()) ++end;
        std::string tmp;
        tmp = tokens.substr(begin, end - begin);
        point = end;
        while (tokens[point] == ' ' and point != tokens.length()) ++point;
        if (point == tokens.length()) hasMore = false;
        return tmp;
    }

    std::string nextKey() {
        int begin = point;
        int end = point;
        while (tokens[end] != '|' and end != tokens.length()) ++end;
        std::string tmp;
        tmp = tokens.substr(begin, end - begin);
        point = end;
        while (tokens[point] == '|' and point != tokens.length()) ++point;
        if (point == tokens.length()) hasMore = false;
        return tmp;
    }

    void su(AccountSystem &account_) {
        std::string tmp = nextToken();
        check_Type1(tmp);
        char UserID_[31];
        memset(UserID_, 0, sizeof(UserID_));
        strcpy(UserID_, tmp.c_str());

        tmp = nextToken();
        char Password_[31];
        memset(Password_, 0, sizeof(Password_));
        if (!tmp.empty()) {
            check_Type1(tmp);
            strcpy(Password_, tmp.c_str());
        }

        if (hasMore) throw std::string("Invalid\n");

        account_.login(UserID_, Password_);
    }

    void register_(AccountSystem &account_) {
        std::string tmp = nextToken();
        check_Type1(tmp);
        char UserID_[31];
        memset(UserID_, 0, sizeof(UserID_));
        strcpy(UserID_, tmp.c_str());

        tmp = nextToken();
        check_Type1(tmp);
        char Password_[31];
        memset(Password_, 0, sizeof(Password_));
        strcpy(Password_, tmp.c_str());

        tmp = nextToken();
        check_Type2(tmp);
        char Username_[31];
        memset(Username_, 0, sizeof(Username_));
        strcpy(Username_, tmp.c_str());

        if (hasMore) throw std::string("Invalid\n");

        account_.register_(UserID_, Password_, Username_);
    }

    void passwd(AccountSystem &account_) {
        std::string tmp = nextToken();
        check_Type1(tmp);
        char UserID_[31];
        memset(UserID_, 0, sizeof(UserID_));
        strcpy(UserID_, tmp.c_str());

        tmp = nextToken();
        std::string tmp1 = nextToken();
        char CurrentPassword_[31];
        memset(CurrentPassword_, 0, sizeof(CurrentPassword_));
        char NewPassword_[31];
        memset(NewPassword_, 0, sizeof(NewPassword_));
        if (tmp1.empty()) {
            check_Type1(tmp);
            strcpy(NewPassword_, tmp.c_str());
        } else {
            check_Type1(tmp);
            check_Type1(tmp1);
            strcpy(CurrentPassword_, tmp.c_str());
            strcpy(NewPassword_, tmp1.c_str());
        }

        if (hasMore) throw std::string("Invalid\n");

        account_.passwd(UserID_, CurrentPassword_, NewPassword_);
    }

    void useradd(AccountSystem &account_) {
        std::string tmp = nextToken();
        check_Type1(tmp);
        char UserID_[31];
        memset(UserID_, 0, sizeof(UserID_));
        strcpy(UserID_, tmp.c_str());

        tmp = nextToken();
        check_Type1(tmp);
        char Password_[31];
        memset(Password_, 0, sizeof(Password_));
        strcpy(Password_, tmp.c_str());

        tmp = nextToken();
        check_Type3(tmp);
        char Privilege_ = tmp[0];

        tmp = nextToken();
        check_Type2(tmp);
        char Username_[31];
        memset(Username_, 0, sizeof(Username_));
        strcpy(Username_, tmp.c_str());

        if (hasMore) throw std::string("Invalid\n");

        account_.useradd(UserID_, Password_, Privilege_, Username_);
    }

    void delete_(AccountSystem &account_) {
        std::string tmp = nextToken();
        check_Type1(tmp);
        char UserID_[31];
        memset(UserID_, 0, sizeof(UserID_));
        strcpy(UserID_, tmp.c_str());

        if (hasMore) throw std::string("Invalid\n");

        account_.delete_(UserID_);
    }

    /*
     * book system
     */

    void show(AccountSystem &account_, BookSystem &book_) {
        std::string tmp = nextToken();
        if (tmp.empty()) {
            book_.show(account_);
            return;
        }
        if (tmp.length() < 6) throw std::string("Invalid\n");
        if (tmp[1] == 'I') {
            char ISBN_[21];
            memset(ISBN_, 0, sizeof(ISBN_));
            strcpy(ISBN_, check_assign_ISBN(tmp).c_str());

            if (hasMore) throw std::string("Invalid\n");

            book_.ISBN_show(ISBN_, account_);
        } else {
            char type[61];
            memset(type, 0, sizeof(type));
            strcpy(type, check_assign(tmp).c_str());

            if (hasMore) throw std::string("Invalid\n");

            if (tmp[1] == 'n') {
                book_.BookName_show(type, account_);
            } else if (tmp[1] == 'a') {
                book_.Author_show(type, account_);
            } else if (tmp[1] == 'k') {
                book_.Keyword_show(type, account_);
            } else throw std::string("Invalid\n");
        }
    }

    void buy(AccountSystem &account_, BookSystem &book_,LogSystem &log_) {
        std::string tmp = nextToken();
        check_Type4(tmp);
        char ISBN_[21];
        memset(ISBN_, 0, sizeof(ISBN_));
        strcpy(ISBN_, tmp.c_str());

        tmp = nextToken();
        int Quantity_ = check_Type6(tmp);
        double cost=0.0;
        book_.buy(ISBN_, Quantity_, account_,cost);
        log_.AddFinance(true,cost);
    }

    void select(AccountSystem &account_, BookSystem &book_) {
        if (account_.login_stack.top().Privilege < '3') throw std::string("Invalid\n");
        std::string tmp = nextToken();
        check_Type4(tmp);
        char ISBN_[21];
        memset(ISBN_, 0, sizeof(ISBN_));
        strcpy(ISBN_, tmp.c_str());

        book_.select(ISBN_, account_);
    }

    void modify(AccountSystem &account_, BookSystem &book_) {
        if (account_.login_stack.top().Privilege < '3') throw std::string("Invalid\n");
//        int index_=account_.User_select[account_.User_select.size() - 1];
        int index_=account_.User_select.top();
        if (index_ == -1) throw std::string("Invalid\n");
        if (!hasMore) throw std::string("Invalid\n");
        std::string tmp = nextToken();
        bool used[5];
        memset(used,false,sizeof(used));
        while (!tmp.empty()) {
            if (tmp.length() < 6) throw std::string("Invalid\n");
            if (tmp[1] == 'I') {
                if (used[0]) throw std::string("Invalid\n");
                char ISBN_[21];
                memset(ISBN_, 0, sizeof(ISBN_));
                strcpy(ISBN_, check_assign_ISBN(tmp).c_str());

                used[0] = true;

                book_.ISBN_modify(ISBN_, account_);
            } else if (tmp[1] == 'k') {
                if (used[3]) throw std::string("Invalid\n");
                char Keyword_[61];
                memset(Keyword_, 0, sizeof(Keyword_));
                if(tmp.length()<11) throw std::string("Invalid\n");
                std::string temp=tmp.substr(1,9);
                if(temp!="keyword=\"" || tmp[tmp.size()-1]!='\"') throw std::string("Invalid\n");
                temp=tmp.substr(10,tmp.length()-11);
                check_Type5(temp);
                strcpy(Keyword_,temp.c_str());
                used[3] = true;

                TokenScanner scanner_key(temp);
                std::string key=scanner_key.nextKey();
                std::unordered_map<std::string,bool> hasKey;
                std::vector<std::string> Key_store;
                while(!key.empty()){
                    if(hasKey.count(key)) throw std::string("Invalid\n");
                    hasKey[key]=true;
                    Key_store.push_back(key);
                    key=scanner_key.nextKey();
                }

                Book tmp_;
                book_.Book_inf.seekg(sizeof(Book) * index_ + sizeof(int));
                book_.Book_inf.read(reinterpret_cast<char *>(&tmp_), sizeof(Book));

                char empty[61];
                memset(empty, 0, sizeof(empty));
                if (strcmp(empty, tmp_.Keyword) != 0) {
                    TokenScanner scanner_key_(tmp_.Keyword);
                    std::string key_=scanner_key_.nextKey();
                    while(!key_.empty()){
                        char key_c[61];
                        strcpy(key_c,key_.c_str());
                        book_.Keyword_map.erase(key_c, tmp_.ISBN, tmp_.tag);
                        key_=scanner_key_.nextKey();
                    }
                }

                strcpy(tmp_.Keyword, Keyword_);
                book_.Book_inf.seekp(sizeof(Book) * index_ + sizeof(int));
                book_.Book_inf.write(reinterpret_cast<char *>(&tmp_), sizeof(Book));

                for(auto &iter:Key_store){
                    char key_c[61];
                    strcpy(key_c,iter.c_str());
                    book_.Keyword_map.insert(key_c, tmp_.ISBN, tmp_.tag);
                }
            } else {
                char type[61];
                memset(type, 0, sizeof(type));
                strcpy(type, check_assign(tmp).c_str());

                if (tmp[1] == 'n') {
                    if (used[1]) throw std::string("Invalid\n");
                    used[1]=true;
                    book_.BookName_modify(type, account_);
                } else if (tmp[1] == 'a') {
                    if (used[2]) throw std::string("Invalid\n");
                    used[2]=true;
                    book_.Author_modify(type, account_);
                } else if (tmp[1] == 'p') {
                    if (used[4]) throw std::string("Invalid\n");
                    used[4]=true;
                    book_.Price_modify(atof(type), account_);
                } else throw std::string("Invalid\n");
            }
//            tmp = nextToken();
//            if(tmp!="|" and !tmp.empty()) throw std::string("Invalid\n");
            tmp = nextToken();
        }
    }


    void import(AccountSystem &account_, BookSystem &book_,LogSystem &log_ ){
        if (account_.login_stack.top().Privilege < '3') throw std::string("Invalid\n");
//        int index_=account_.User_select[account_.User_select.size() - 1];
        int index_=account_.User_select.top();
        if (index_ == -1) throw std::string("Invalid\n");

        std::string tmp = nextToken();
        int Quantity_ = check_Type6(tmp);

        tmp = nextToken();
        check_Type7(tmp);
        double TotalCost_ = atof(tmp.c_str());

        book_.import(Quantity_,TotalCost_,account_);
        log_.AddFinance(false,TotalCost_);
    }

    void show_finance(AccountSystem &account_,LogSystem &log_) {
        if (account_.login_stack.top().Privilege < '7') throw std::string("Invalid\n");
        std::string tmp = nextToken();
        if(tmp.empty()){
            log_.ShowFinance(-1);
            return;
        }
        int Quantity_ = check_Type6(tmp);
        log_.ShowFinance(Quantity_);
    }

    void log(AccountSystem &account_, BookSystem &book_) {
        if (account_.login_stack.top().Privilege < '7') throw std::string("Invalid\n");

    }

};

#endif //INC_1_6BOOKSTORE_TOKENSCANNER_H
