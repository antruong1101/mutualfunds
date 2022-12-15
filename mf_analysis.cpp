/*
 An Truong
 Assignment Make Up
 Compiler: Xcode
 OS: Mac
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <ostream>
#include <ctime>
#include <cstdlib>

const unsigned int TIME_PERIOD = 100;
const unsigned int INITIAL_PRICE = 10000;

using namespace std;

class MutualFund;
class Date {
    unsigned int month, day, year;
    static unsigned DaysPerMonth[13];
public:
    Date();
    Date(unsigned,unsigned,unsigned);
    Date getSellDate(const Date&);
    unsigned int getMonth() const {return month;}
    unsigned int getDay() const {return day;}
    unsigned int getYear() const {return year;}
    void increment();
    void setDate(unsigned int, unsigned int, unsigned int);
    void getRandomDate(const Date&);
    bool operator< (const Date&) const;
    bool operator> (const Date&) const;
    bool operator== (const Date&) const;
    bool operator!= (const Date&) const;
friend ostream& operator<< (ostream&, const Date&);
};

unsigned Date::DaysPerMonth[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

Date :: Date() { //default
    month = 99;
    day = 99;
    year = 9999;
    Date lastDate = Date(11,11,2020); //last reported share date on file
    while (*this > lastDate) {
        month = rand() % 12 + 1;
        day = rand() % DaysPerMonth[month] + 1;
        year = 2005 + (rand() % (2020 - 2005 + 1));
    }
}

Date :: Date(unsigned m, unsigned d, unsigned y) : month(m), day(d), year(y) {}

Date Date::getSellDate(const Date& d) {
    Date buyDate = Date(month,day,year);
    for (size_t i = 0; i < 100; i++) {buyDate.increment();} //let 100 days pass
    unsigned int Nmonth = 99, Nday = 99, Nyear = 9999;
    
    Date lastDate = d; //last reported share date on file
    Date mocSellDate(Nmonth, Nday, Nyear); //subject to be changed
    
    //before incremented date and before last date on file
    while ((mocSellDate > buyDate) || (mocSellDate > lastDate && mocSellDate != buyDate)) {
        Nmonth = rand() % 12 + 1;
        Nday = rand() % DaysPerMonth[month] + 1;
        Nyear  = year + (rand() % (2020 - year + 1));
        mocSellDate.setDate(Nmonth, Nday, Nyear);
    }
    
    return mocSellDate;
}

void Date::getRandomDate (const Date& d) {
    month = 99;
    day = 99;
    year = 9999;
    Date lastDate = d; //last reported share date on file
    while (*this > lastDate) {
        month = rand() % 12 + 1;
        day = rand() % DaysPerMonth[month] + 1;
        year = 2005 + (rand() % (2020 - 2005 + 1));
    }
}

bool Date::operator<(const Date& d) const{
    
    if (year < d.getYear()) return true; //check year
    else if (year > d.getYear()) return false;
    
    if (month < d.getMonth()) return true; //check month
    else if (month > d.getMonth()) return false;
    
    if (day < d.getDay()) return true; //check day
    else if (day > d.getDay()) return false;
    
    else return false;

}

bool Date::operator>(const Date& d) const{
    
    if (year > d.getYear()) return true; //check year
    else if (year < d.getYear()) return false;
    
    if (month > d.getMonth()) return true; //check month
    else if (month < d.getMonth()) return false;
    
    if (day > d.getDay()) return true; //check day
    else if (day < d.getDay()) return false;
    
    else return false;
    
}

bool Date::operator==(const Date& d) const {
    if (month == d.getMonth() && day == d.getDay() && year == d.getYear()) return true;
    
    else return false;
}

bool Date::operator!=(const Date& d) const {
    return !(*this == d);
}

void Date::setDate(unsigned int m, unsigned int d, unsigned int y) {
    month = m;
    day = d;
    year = y;
}

void Date::increment() {
    day++;
    bool leapYear = false;
    if (year % 4 == 0 || year % 400 == 0) leapYear = true; //checks for leapyear
    
    if (leapYear && month == 2) { //account for leap year
        if (day > DaysPerMonth[month] + 1) {
            month++;
            day = 1;
        }
    }
    
    else {
        if (day > DaysPerMonth[month]) {
            month++;
            day = 1;
            if (month > 12) {
                month = 1;
                year++;
            }
        }
    }
}

ostream& operator<< (ostream& s, const Date& d) {
    s << setfill('0');
    s << setw(2) << d.getMonth() << "/" << setw(2) << d.getDay() << "/" << d.getYear();
    s << setfill(' ');
    
    return s;
}


class NAV { //Net Asset Value: get date and share value
    Date date;
    float sharePrice;
public:
    NAV(Date d, float sp) : date(d), sharePrice(sp) {}
    Date getNAVdate() const {return date;}
    float getSharePrice() const {return sharePrice;}
};


class DIV {
    Date date;
    float amount;
public:
    DIV(Date d, float a) : date(d), amount(a) {}
    Date getDivDate() const {return date;}
    float getDivAmount() const {return amount;}
    void printDiv() const;
};

void DIV::printDiv() const{ //testing purposes
    cout << date << " : " << amount << endl;
}


class MutualFund {
    NAV** navs;
    DIV** divs;
    Date buyDate;
    Date sellDate;
    Date lastDate; //last date noted on sharefile
    float balance = 10000;
    float buyPrice, sellPrice;
    float numShares = 0;
    unsigned int NAVcount = 0, DIVcount = 0;
public:
    MutualFund(const string&, const string&); //read input file
    ~MutualFund(); //dtor
    Date extractDate(const string&);
    Date genValidBuyDate();
    Date genValidSellDate();
    float extractSharePrice(const string&);
    float extractDivPrice(const string&);
    float getSharePrice(const Date&) const;
    float getDivPrice(const Date&) const;
    float getSellPrice(const Date&) const;
    float getSellTotal() const;
    Date getlastDate() const {return lastDate;}
    void sortDivs();
    void reset(); //in order to cycle through new dates and balance
    void printDivs() const;
    void reinvest(const Date&);
    void printStatus() const; //for test purposes
    void processFunds();
    void setDates(const Date&, const Date&); //works if checkDates is true;
    bool checkDates(const Date&, const Date&);
};

void MutualFund::reset() {
    balance = 10000;
    numShares = 0;
}

Date MutualFund::extractDate(const string& line) {
    unsigned int month = 0, day = 0, year = 0;
    string data, strMonth, strDay, strYear;
    size_t commaLoc, yearLoc, monthLoc, dayLoc;
    
    //substr dates of out argument
    commaLoc = line.find(",");
    data = line.substr(0,commaLoc);
    
    yearLoc = data.find("-");
    strYear = data.substr(0, yearLoc);
    year = stoi(strYear);
    
    monthLoc = data.find("-",yearLoc+1);
    strMonth = data.substr(yearLoc+1, 2);
    month = stoi(strMonth);
    
    dayLoc = data.find("-", monthLoc+1);
    strDay = data.substr(monthLoc+1, 2);
    day = stoi(strDay);
    
    return Date(month, day, year);
}
 
float MutualFund::extractSharePrice(const string& line) {
    float sharePrice;
    size_t firstCommaLoc, secondCommaLoc, thirdCommaLoc; //refers to commas in the back
    string strShare;
    
    //skim through first three commas to get closed share price
    firstCommaLoc = line.rfind(","); //skips the last comma
    strShare = line.substr(0,firstCommaLoc);
    
    secondCommaLoc = strShare.rfind(",");
    strShare = strShare.substr(0,secondCommaLoc);
    
    thirdCommaLoc = strShare.rfind(",");
    strShare = strShare.substr(thirdCommaLoc+1, secondCommaLoc);
    
    //convert str to float
    sharePrice = stof(strShare);
    
    return sharePrice;
}

float MutualFund::extractDivPrice(const string& line) {
    float divPrice;
    size_t commaLoc;
    string strDiv;
    
    //skim out comma
    commaLoc = line.rfind(",");
    strDiv = line.substr(commaLoc+1);
    
    //convert str to float
    divPrice = stof(strDiv);
    //cout << strDiv << endl;
    
    return divPrice;
}

float MutualFund::getSharePrice(const Date& d) const { //binary search
    
    int low, high, middle;
    low  = 0;
    high = NAVcount - 1;
    
    while (low <= high) {
        middle = (low + high) / 2;
        if (d < navs[middle]->getNAVdate()) {
            high = middle - 1;
        }
        
        else if (d > navs[middle]->getNAVdate()) {
            low = middle + 1;
        }
        
        else {
            return navs[middle]->getSharePrice();
        }
    }
    return -1;
}

float MutualFund::getDivPrice(const Date& d) const { //binary search
    
    int low, high, middle;
    low  = 0;
    high = DIVcount - 1;
    
    while (low <= high) {
        middle = (low + high) / 2;
        
        if (d < divs[middle]->getDivDate()) {
            high = middle - 1;
        }
        
        else if (d > divs[middle]->getDivDate()) {
            low = middle + 1;
        }
        
        else {
            return divs[middle]->getDivAmount();
        }
    }
    return -1;
}

Date MutualFund::genValidBuyDate() {
    //get date with share price
    buyDate = Date();
    buyPrice = getSharePrice(buyDate);
    
    //makes sure the date has a share price
    while (1) {
        if (buyPrice != -1) break;
        Date randDate = Date();
        buyDate = randDate; //update buyDate
        buyPrice = getSharePrice(buyDate);
    }
    return buyDate;
}

Date MutualFund::genValidSellDate() {
    sellDate = buyDate.getSellDate(lastDate);
    sellPrice = getSharePrice(sellDate);
    
    while (1) {
        if (sellPrice != -1) break;
        Date randDate = buyDate.getSellDate(lastDate);
        sellDate = randDate; //update buyDate
        sellPrice = getSharePrice(buyDate);
    }
    return sellDate;
}

void MutualFund::reinvest(const Date& d) {
    float madeFromDivs, divPrice, sharePrice, sharesBought;
    
    divPrice = getDivPrice(d);
    sharePrice = getSharePrice(d);
    
    madeFromDivs = divPrice * numShares;
    sharesBought  = madeFromDivs / sharePrice;
    numShares += sharesBought;
}

MutualFund :: MutualFund(const string& NAVfile, const string& DIVfile) {
    //srand(static_cast<unsigned>(time(0)));
    string buffer;
    
    ifstream NAVset, DIVset;
    
    //first run to get number of elements
    NAVset.open(NAVfile);
    DIVset.open(DIVfile);
    
    if (NAVset.fail() && DIVset.fail()) {
        cerr << "Error opening file(s)." << endl;
        exit(1);
    }

    //get count of NAV
    getline(NAVset, buffer); //buffer out header
    while (!NAVset.eof()) {
        getline(NAVset, buffer);
        if (buffer.size() < 1) break; //eof check
        NAVcount++;
    }
    NAVset.close();
    
    getline(DIVset, buffer); //buffer out header
    while (!DIVset.eof()) {
        getline(DIVset, buffer);
        if (buffer.size() < 1) break; //eof check
        DIVcount++;
    }
    DIVset.close();
    
    //allocate memory based on counts
    navs = new NAV* [NAVcount];
    divs = new DIV* [DIVcount];
    
    //second run to store elements
    size_t i;
    NAVset.open(NAVfile);
    DIVset.open(DIVfile);
    
    if (NAVset.fail() && DIVset.fail()) {
        cerr << "Error opening file(s)." << endl;
        exit(1);
    }

    //get count of NAV
    getline(NAVset, buffer); //buffer out header
    for (i = 0; i < NAVcount; i++) {
        getline(NAVset, buffer);
        Date navDate= extractDate(buffer);
        float sharePrice = extractSharePrice(buffer);
        
        navs[i] = new NAV(navDate, sharePrice);
    }
    NAVset.close();
    
    //get last date from navs
    lastDate = navs[NAVcount - 1]->getNAVdate();
    //cout << lastDate << endl;
    
    getline(DIVset, buffer); //buffer out header
    for (i = 0; i < DIVcount; i++) {
        getline(DIVset, buffer);
        Date divDate = extractDate(buffer);
        float divPrice = extractDivPrice(buffer);
        
        divs[i] = new DIV(divDate, divPrice);
    }
    DIVset.close();
    sortDivs(); //sort divs
}

bool MutualFund::checkDates(const Date& buy, const Date& sell) {
    float sharePrice, sellSharePrice;
    //get buy share price
    sharePrice = getSharePrice(buy);
    
    //get sell share price
    sellSharePrice = getSharePrice(sell);
    
    //makes sure the dates have share prices
    if (sharePrice != -1 && sellSharePrice  != - 1 && buy < sell) return true;
    
    else return false;
}

void MutualFund::setDates(const Date& buy, const Date& sell) {
    buyDate = buy;
    sellDate = sell;
    buyPrice = getSharePrice(buyDate);
    sellPrice = getSharePrice(sellDate);
}

void MutualFund::processFunds() {
    
    //get shares from buyDate
    numShares = balance / buyPrice;
    balance -= (numShares * buyPrice);
    
    while (buyDate != sellDate) {
        buyDate.increment();
        float divPrice = getDivPrice(buyDate);
        if (divPrice != -1) {
            reinvest(buyDate);
        }
    }
}

float MutualFund::getSellTotal() const {
    return numShares * sellPrice;
}

void MutualFund::printStatus() const {
    cout << "Balance: " << balance << endl;
    cout << "Buy Date: " << buyDate << endl;
    cout << "Sell Date: " << sellDate << endl;
    cout << "Share Price: " << buyPrice << endl;
    cout << "Sell Share Price: " << sellPrice << endl;
    cout << "Shares: " << numShares << endl;
}

void MutualFund::printDivs() const { // for test purposes
    for (size_t i = 0; i < DIVcount; i++) {
        divs[i]->printDiv();
    }
}

void MutualFund::sortDivs() { //selection sort
    size_t smallestIndex, i, j;
    DIV* temp;
    
    for (i = 0; i < DIVcount - 1; i++) {
        smallestIndex = i;
        
        for (j = i + 1; j < DIVcount; j++) {
            Date date1 = divs[j]->getDivDate();
            Date date2 = divs[smallestIndex]->getDivDate();
            
            if (date1 < date2) {
                smallestIndex = j;
            }
        }
        
        if (i != smallestIndex) {
            temp = divs[i];
            divs[i] = divs[smallestIndex];
            divs[smallestIndex] = temp;
        }
    }
}

MutualFund :: ~MutualFund() {
    size_t i;
    //release navs memory
    for (i = 0; i < NAVcount; i++) {
        delete navs[i];
        navs[i] = nullptr;
    }
    delete [] navs;
    
    //release divs memory
    for (i = 0; i < DIVcount; i++) {
        delete divs[i];
        divs[i] = nullptr;
    }
    delete [] divs;
}

const string VFIAXfile = "VFIAX.csv";
const string VFIAXdivFile = "VFIAXdiv.csv";
const string VTSAXfile = "VTSAX.csv";
const string VTSAXdivFile = "VTSAXdiv.csv";
const string reportFile = "report.txt";

void lookForDates(MutualFund&, MutualFund&);

int main() {
    
    //get mutual funds
    MutualFund VFIAX(VFIAXfile, VFIAXdivFile);
    MutualFund VTSAX(VTSAXfile, VTSAXdivFile);
    
    //initialize buy and sell dates
    Date buy;
    Date sell;
    
    //initialize values and gains of mutual funds
    float VTSAXval, VTSAXgain, VFIAXval, VFIAXgain, difference;
    float VTSAXtotal = 0, VFIAXtotal = 0, VTSAXavg = 0, VFIAXavg = 0;
    
    cout << right << setw(10) << "Buy Date" << right << setw(13) << "Sell Date" << right << setw(12) <<
    "VTSAX Value" << right << setw(12) << "VTSAX Gain" << right << setw(12) << "VFIAX Value" <<
    right << setw(12) << "VFIAX Gain" << right << setw(12) << "VTSAX-VFIAX" << endl;
    
    for (size_t i = 0; i < TIME_PERIOD; i++) {
       
        //check that both mutual funds can use the same date
        while (!(VFIAX.checkDates(buy, sell) && VTSAX.checkDates(buy, sell))) {
            buy = VFIAX.genValidBuyDate();
            sell = VFIAX.genValidSellDate();
        }
        //makes sure both funds get the same date
        VFIAX.setDates(buy, sell);
        VTSAX.setDates(buy, sell);
        
        //process funds
        VFIAX.processFunds();
        VTSAX.processFunds();
        
        //calculate values and gains
        VFIAXval = VFIAX.getSellTotal();
        VFIAXgain = VFIAXval - INITIAL_PRICE;
        VFIAXtotal += VFIAXgain;
        
        VTSAXval = VTSAX.getSellTotal();
        VTSAXgain =  VTSAXval - INITIAL_PRICE;
        VTSAXtotal += VTSAXgain;

        //get difference between the two
        difference = VTSAXgain - VFIAXgain;
        
        //print everything
        cout << fixed << setprecision(2);
        cout << buy << "   " << sell << setfill(' ') << right << setw(12)
        << VTSAXval << right << setw(12) << VTSAXgain << right << setw(12) << VFIAXval <<
        right << setw(12) << VFIAXgain << right << setw(12) << difference << endl;
        
        //reset data members and dates for next run
        buy.getRandomDate(VFIAX.getlastDate());
        sell.getRandomDate(VFIAX.getlastDate());
        VFIAX.reset();
        VTSAX.reset();
    }
    
    //calculate mutual fund averages
    VFIAXavg = VFIAXtotal / TIME_PERIOD;
    VTSAXavg = VTSAXtotal / TIME_PERIOD;
    
    cout << "---------------" << endl;
    cout << "Average gain for VTSAX = " << VTSAXavg << endl;
    cout << "Average gain for VFIAX = " << VFIAXavg << endl;
    cout << "---------------" << endl;
    
    //lookForDates(VFIAX, VTSAX); for testing purposes
    
    return 0;
}

void lookForDates(MutualFund& mf1, MutualFund& mf2) {
    //initialize values and gains of mutual funds
    float VTSAXval, VTSAXgain, VFIAXval, VFIAXgain, difference;
    
    string buyDateStr, sellDateStr;
    string BuyMon, BuyDay, BuyYear, SellMon, SellDay, SellYear;
    size_t monLoc, dayLoc;
    unsigned int mon,day,year;
    Date buy, sell;
    
    //prompt user for buy and sell dates
    cout << "Buy Date (mm/dd/yyyy): ";
    cin >> buyDateStr;
    cout << "\nSell Date (mm/dd/yyyy): ";
    cin >> sellDateStr;
    
    //get dates from string
    //buy date
    monLoc = buyDateStr.find("/");
    BuyMon = buyDateStr.substr(0,monLoc);
    dayLoc = buyDateStr.find("/", monLoc+1);
    BuyDay = buyDateStr.substr(monLoc+1,2);
    BuyYear = buyDateStr.substr(dayLoc+1, 4);
    
    mon  = stoi(BuyMon);
    day = stoi(BuyDay);
    year = stoi(BuyYear);
    
    buy = Date(mon, day, year);
    
    //sell date
    monLoc = sellDateStr.find("/");
    BuyMon = sellDateStr.substr(0,monLoc);
    dayLoc = sellDateStr.find("/", monLoc+1);
    BuyDay = sellDateStr.substr(monLoc+1,2);
    BuyYear = sellDateStr.substr(dayLoc+1, 4);
    
    mon  = stoi(BuyMon);
    day = stoi(BuyDay);
    year = stoi(BuyYear);
    
    sell = Date(mon, day, year);
    
    mf1.setDates(buy, sell);
    mf2.setDates(buy, sell);
    
    mf1.processFunds();
    mf2.processFunds();
    
    //calculate values and gains
    VFIAXval = mf1.getSellTotal();
    VFIAXgain = VFIAXval - INITIAL_PRICE;
    
    VTSAXval = mf2.getSellTotal();
    VTSAXgain =  VTSAXval - INITIAL_PRICE;

    //get difference between the two
    difference = VTSAXgain - VFIAXgain;
    
    //print everything
    cout << right << setw(10) << "Buy Date" << right << setw(13) << "Sell Date" << right << setw(12) <<
    "VTSAX Value" << right << setw(12) << "VTSAX Gain" << right << setw(12) << "VFIAX Value" <<
    right << setw(12) << "VFIAX Gain" << right << setw(12) << "VTSAX-VFIAX" << endl;
    cout << fixed << setprecision(2);
    cout << buy << "   " << sell << setfill(' ') << right << setw(12)
    << VTSAXval << right << setw(12) << VTSAXgain << right << setw(12) << VFIAXval <<
    right << setw(12) << VFIAXgain << right << setw(12) << difference << endl;
}
