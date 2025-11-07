#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
using namespace std;

class Movie {
private:
    string title;
    string time;
    int seats;

public:
    
    Movie() { title = ""; time = ""; seats = 0; }

    
    Movie(const string &t, const string &ti, int s) : title(t), time(ti), seats(s) {}

    
    Movie(const Movie &m) {
        title = m.title;
        time = m.time;
        seats = m.seats;
    }

    
    inline string getTitle() const { return title; }
    inline string getTime() const { return time; }
    inline int getSeats() const { return seats; }

    
    void setSeats(int s) { this->seats = s; }

    
    bool bookSeats(int cnt) {
        if (cnt <= 0) return false;
        if (seats >= cnt) {
            seats -= cnt;
            return true;
        }
        return false;
    }

    bool bookSeats(string seatText) {
        int cnt = atoi(seatText.c_str());
        return bookSeats(cnt);
    }

    
    void show() const {
        cout << "Movie: " << title << " | Time: " << time << " | Seats left: " << seats << "\n";
    }

    
    string toCSV() const {
        char buf[200];
        sprintf(buf, "%s,%s,%d", title.c_str(), time.c_str(), seats);
        return string(buf);
    }

    
    bool fromCSV(const string &line) {
        size_t p1 = line.find(",");
        if (p1 == string::npos) return false;
        size_t p2 = line.find(",", p1 + 1);
        if (p2 == string::npos) return false;
        title = line.substr(0, p1);
        time = line.substr(p1 + 1, p2 - p1 - 1);
        seats = atoi(line.substr(p2 + 1).c_str());
        return true;
    }

    
    ~Movie() {}


    friend void showMovieDetails(const Movie &m);
};


void showMovieDetails(const Movie &m) {
    cout << "[Friend] Movie Details => Title: " << m.title
         << ", Time: " << m.time << ", Available Seats: " << m.seats << "\n";
}


vector<Movie> loadMovies(const char *filename = "movielist.txt") {
    vector<Movie> list;
    ifstream fin(filename);
    if (!fin) {
        cout << "File not found, creating default movies...\n";
        return list;
    }

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        Movie m;
        if (m.fromCSV(line)) {
            list.push_back(Movie(m));  
        }
    }
    fin.close();
    return list;
}


void saveMovies(const vector<Movie> &list, const char *filename = "movielist.txt") {
    ofstream fout(filename);
    if (!fout) throw runtime_error("Cannot open file for writing!");
    for (int i = 0; i < (int)list.size(); ++i)
        fout << list[i].toCSV() << "\n";
    fout.close();
}


void showBookingsFromFile(const char *filename = "bookings_movie.txt") {
    ifstream fin(filename);
    if (!fin) {
        cout << "No previous bookings found.\n";
        return;
    }
    string line;
    cout << "\n--- Past Bookings ---\n";
    while (getline(fin, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find(",");
        size_t p2 = line.find(",", p1 + 1);
        size_t p3 = line.find(",", p2 + 1);
        string cust = line.substr(0, p1);
        string mov = line.substr(p1 + 1, p2 - p1 - 1);
        string tim = line.substr(p2 + 1, p3 - p2 - 1);
        string seat = line.substr(p3 + 1);
        cout << "Customer: " << cust << " | Movie: " << mov
             << " | Time: " << tim << " | Seats: " << seat << "\n";
    }
    fin.close();
}


int main() {
    try {
        vector<Movie> movies = loadMovies();

       
        if (movies.empty()) {
            movies.push_back(Movie("Pushpa 2", "10:00 AM", 50));
            movies.push_back(Movie("Dabang", "2:00 PM", 40));
            movies.push_back(Movie("Thama", "6:00 PM", 60));
            movies.push_back(Movie("Kantara", "9:00 PM", 70));
        }

        int choice;
        do {
            cout << "\n=== Movie Ticket Booking System ===\n";
            cout << "1. Show Movies\n2. Book Ticket\n3. Show Bookings\n4. Save & Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (choice == 1) {
                cout << "\n--- Available Movies ---\n";
                for (int i = 0; i < (int)movies.size(); ++i) {
                    cout << (i + 1) << ". ";
                    movies[i].show();
                }
            } 
            else if (choice == 2) {
                cout << "\n--- Available Movies ---\n";
                for (int i = 0; i < (int)movies.size(); ++i) {
                    cout << (i + 1) << ". ";
                    movies[i].show();
                }
                int ch; 
                cout << "Choose movie #: "; 
                cin >> ch;

                if (ch < 1 || ch > (int)movies.size()) {
                    cout << "Invalid selection.\n";
                    continue;
                }

                string seats; 
                cout << "Enter number of seats: "; 
                cin >> seats;

                if (!movies[ch - 1].bookSeats(seats)) {
                    cout << "Not enough seats.\n";
                    continue;
                }

                cin.ignore(200, '\n');
                string cust; 
                cout << "Enter customer name: "; 
                getline(cin, cust);

                ofstream fout("bookings_movie.txt", ios::app);
                if (!fout) throw runtime_error("Cannot open booking file!");
                fout << cust << "," << movies[ch - 1].getTitle() << ","
                     << movies[ch - 1].getTime() << "," << seats << "\n";
                fout.close();

                cout << "Booking saved successfully!\n";
            } 
            else if (choice == 3) {
                showBookingsFromFile();
            } 
            else if (choice == 4) {
                saveMovies(movies);
                cout << "Movies saved. Exiting...\n";
            } 
            else {
                cout << "Invalid choice.\n";
            }
        } while (choice != 4);
    } 
    catch (exception &e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}

