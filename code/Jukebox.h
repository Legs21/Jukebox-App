#ifndef JUKEBOX_H__
#define JUKEBOX_H__

#include <memory>
#include <string>

using namespace std;

struct node {
    node* next; // points to next node/song. NULL if end of jukebox and no continuous play
    node* previous; // points to previous node/song. NULL if first song in jukebox and no continuous play
    string title; // title of the song playing
    string playlist; // playlist the song is apart of
};

class Jukebox {
public:
    // Constructor
    Jukebox();

    // Deconstructor
    ~Jukebox();

    // creates a new node to hold a song, both pointers are at NULL and the node contains 
    // title and playlist information
    node* init_node(string title, string playlist);

    // returns the title of the song that is currently playing, starts at the first
    // song on the Jukebox
    string playing_song();

    // returns the playlist currently running on the Jukebox
    string playlist();

    // skips to next song in playlist, if end of playlist, goes to next playlist
    string next_song();

    // plays previous song, if first song in the playlist, plays last song of previous playlist
    string previous_song();

    // skips to the first song in the next playlist
    void next_playlist();

    // moves entire playlist to a new spot in between the two playlists
    // specified in the arguments of the function
    // it also sets the current song to the first song of the moved playlist
    void move_playlist(string playlist_to_move, string playlist_1, string playlist_2);

    // takes you to the first song of your current playlist
    void first_track_this_playlist();

    //takes you to the first track of the previous playlist if currently
    // playing first song of current playlist
    void first_track_previous_playlist();

    // switch that when "on", means the last song goes right into the first
    // of the jukebox, and doesn't when "off"
    bool juke_repeat(string repeat);

    // sets the first song in the Jukebox
    void set_first(node* first);

    // sets current song being played
    void set_current(node* current);

    //gets the first song in the Jukebox
    node* get_first();

private:
    node* first_song;
    node* current_song;
    string loop;

};

#endif // JUKEBOX_H__