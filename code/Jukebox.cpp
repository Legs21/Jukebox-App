#include "Jukebox.h"

// constructor
Jukebox::Jukebox() {
    first_song = NULL;
    current_song = NULL;
    loop = "off";
}

// deconstructor
Jukebox::~Jukebox() {}

node* Jukebox::init_node(string title, string playlist) {
    node* ret(new node);
    ret->title = title;
    ret->playlist = playlist;
    ret->next = NULL;
    ret->previous = NULL;
    return ret;
}

string Jukebox::playing_song() {
return current_song->title;
}

string Jukebox::playlist() {
return current_song->playlist;
}

string Jukebox::next_song() {
    if (current_song->next != NULL) {
        current_song = current_song->next;
        return "Playing next song";
    }
    else {
        return "End of Jukebox";
    }
}

string Jukebox::previous_song() {
    if (current_song->previous != NULL) {
        current_song = current_song->previous;
        return "Playing previous song";
    }
    else {
        return "First song in Jukebox";
    }
}

void Jukebox::next_playlist() {
    // making a cursor so that if its the last playlist (aka repeat is off)
    // we can keep the current song
    node* cursor = current_song;
    while (cursor->next != NULL && 
    cursor->playlist == cursor->next->playlist) {
        cursor = cursor->next;
    }
    if (cursor->next != NULL) {
        current_song = cursor->next;
    }
}

void Jukebox::move_playlist(string playlist_to_move, string playlist_1,
string playlist_2) {
    // storing previous loop value and turning loop on
    string original = loop;
    juke_repeat("on");
    // finding the nodes I need to reconnect
    node* last_song_playlist_1 = first_song;
    while (last_song_playlist_1->playlist != playlist_1) {
        last_song_playlist_1 = last_song_playlist_1->next;
    }
    while (last_song_playlist_1->playlist == playlist_1) {
        last_song_playlist_1 = last_song_playlist_1->next;
    }
    node* first_song_playlist_2 = first_song;
    while (first_song_playlist_2->playlist != playlist_2) {
        first_song_playlist_2 = first_song_playlist_2->next;
    }
    node* first_song_to_move = first_song;
    while (first_song_to_move->playlist != playlist_to_move) {
        first_song_to_move = first_song_to_move->next;
    }
    node* loose_end_1 = first_song_to_move->previous;
    node* last_song_to_move = first_song_to_move;
    while (last_song_to_move->playlist == playlist_to_move) {
        last_song_to_move = last_song_to_move->next;
    }
    node* loose_end_2 = last_song_to_move->next;

    last_song_playlist_1->next = NULL;
    last_song_to_move->next = NULL;
    first_song_playlist_2->previous = NULL;
    first_song_to_move->previous = NULL;
    // reassigning next and previous to move playlist
    last_song_playlist_1->next = first_song_to_move;
    first_song_to_move->previous = last_song_playlist_1;

    last_song_to_move->next = first_song_playlist_2;
    first_song_playlist_2->previous = last_song_to_move;

    loose_end_1->next = loose_end_2;
    loose_end_2->previous = loose_end_1;

    juke_repeat("off");

    while (first_song->previous != NULL) {
        first_song = first_song->previous;
    }
    set_current(first_song_to_move);
    juke_repeat(original);
}

void Jukebox::first_track_this_playlist() {
    while (current_song->playlist == current_song->previous->playlist) {
        current_song = current_song->previous;
    }
}

void Jukebox::first_track_previous_playlist() {
    // checking the function can run
    if (current_song->previous->playlist == current_song->playlist) {
        return;
    }
    // making a cursor to iterate
    string new_playlist = current_song->previous->playlist;
    node* cursor = current_song;

    while (cursor->previous != NULL && 
    cursor->previous->playlist == new_playlist) {
        cursor = cursor->previous;
    }
    // setting current song to first song of previous playlist
    current_song = cursor;
}

bool Jukebox::juke_repeat(string repeat) {
    // making a cursor to find the last song in the jukebox without effecting
    // the current song
    node* cursor = current_song;
    if (repeat == "on") {
        // checking to see if repeat is already on
        if (loop == "on") {
            return true;
        }
        // finding end of jukebox
        while (cursor->next != NULL) {
        cursor = cursor->next;
        }
        // connecting last song in jukebox with first song to make a loop
        cursor->next = first_song;
        first_song->previous = cursor;
        loop = "on";
        return true;
    }
    // severing the loop using the first song in the jukebox
    else if (repeat == "off") {
        if (loop == "off") {
            return false;
        }
        cursor = first_song->previous;
        cursor->next = NULL;
        first_song->previous = NULL;
        loop = "off";
        return false;
    }
}

void Jukebox::set_first(node* first) {
    first_song = first;
}

void Jukebox::set_current(node* current) {
    current_song = current;
}

node* Jukebox::get_first() {
    return first_song;
}