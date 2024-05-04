#include "../code/Jukebox.h"
#include <gtest/gtest.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class test_Jukebox : public ::testing::Test {
protected:
  // This function runs only once before any TEST_F function
  static void SetUpTestCase() {
    std::ofstream outgrade("./total_grade.txt");
    if (outgrade.is_open()) {
      outgrade.clear();
      outgrade << (int)0;
      outgrade.close();
    }
  }

  // This function runs after all TEST_F functions have been executed
  static void TearDownTestCase() {
    std::ofstream outgrade("./total_grade.txt");
    if (outgrade.is_open()) {
      outgrade.clear();
      //outgrade << (int)std::ceil(100 * total_grade / max_grade);
      outgrade.close();
      //std::cout << "Total Grade is : "
        //        << (int)std::ceil(100 * total_grade / max_grade) << std::endl;
    }
  }

  void add_points_to_grade(double points) {
    if (!::testing::Test::HasFailure()) {
      total_grade += points;
    }
  }

  // this function runs before every TEST_F function
  void SetUp() override {}

  // this function runs after every TEST_F function
  void TearDown() override {
    std::ofstream outgrade("./total_grade.txt");
    if (outgrade.is_open()) {
      outgrade.clear();
     // outgrade << (int)std::ceil(100 * total_grade / max_grade);
      outgrade.close();
    }
  }

  static double total_grade;
  static double max_grade;
};

node* build_four_song_jukebox_helper(string song_one, string song_two,
string song_three, string song_four, string playlist_one, string playlist_two) {
    node* top (new node);
    top->title = song_one;
    top->playlist = playlist_one;
    top->previous = NULL;
    top->next = new node;
    top->next->title = song_two;
    top->next->playlist = playlist_one;
    top->next->previous = top;
    top->next->next = new node;
    top->next->next->title = song_three;
    top->next->next->playlist = playlist_two;
    top->next->next->previous = top->next;
    top->next->next->next = new node;
    top->next->next->next->title = song_four;
    top->next->next->next->playlist = playlist_two;
    top->next->next->next->previous = top->next->next;
    top->next->next->next->next = NULL;
    return top;
}

node* build_six_song_jukebox_helper(string song_one, string song_two,
string song_three, string song_four, string song_five, string song_six,
 string playlist_one, string playlist_two, string playlist_three) {
    node* top (new node);
    top->title = song_one;
    top->playlist = playlist_one;
    top->previous = NULL;
    top->next = new node;
    top->next->title = song_two;
    top->next->playlist = playlist_one;
    top->next->previous = top;
    top->next->next = new node;
    top->next->next->title = song_three;
    top->next->next->playlist = playlist_two;
    top->next->next->previous = top->next;
    top->next->next->next = new node;
    top->next->next->next->title = song_four;
    top->next->next->next->playlist = playlist_two;
    top->next->next->next->previous = top->next->next;
    top->next->next->next->next = new node;
    top->next->next->next->next->title = song_five;
    top->next->next->next->next->playlist = playlist_three;
    top->next->next->next->next->previous = top->next->next->next;
    top->next->next->next->next->next = new node;
    top->next->next->next->next->next->title = song_six;
    top->next->next->next->next->next->playlist = playlist_three;
    top->next->next->next->next->next->previous = top->next->next->next->next;
    top->next->next->next->next->next->next = NULL;
    return top;
}

TEST_F(test_Jukebox, TestSongMovement) {
    Jukebox punk;
    node* top = build_four_song_jukebox_helper("Black Parade", "Helena",
    "Everlong", "My Hero", "MCR", "Foo Fighters");
    punk.set_first(top);
    punk.set_current(top);

    punk.next_song();

    ASSERT_EQ("Helena", punk.playing_song());
    ASSERT_EQ("MCR", punk.playlist());

    punk.next_song();

    ASSERT_EQ("Everlong", punk.playing_song());
    ASSERT_EQ("Foo Fighters", punk.playlist());

    punk.previous_song();

    ASSERT_EQ("Helena", punk.playing_song());
    ASSERT_EQ("MCR", punk.playlist());
}

TEST_F(test_Jukebox, TestPlaylistMovement) {
    Jukebox punk;
    node* top = build_four_song_jukebox_helper("Black Parade", "Helena",
    "Everlong", "My Hero", "MCR", "Foo Fighters");
    punk.set_first(top);
    punk.set_current(top);

    punk.next_playlist();

    ASSERT_EQ("Everlong", punk.playing_song());
    ASSERT_EQ("Foo Fighters", punk.playlist());

    punk.next_song();
    punk.first_track_this_playlist();

    ASSERT_EQ("Everlong", punk.playing_song());
    ASSERT_EQ("Foo Fighters", punk.playlist());

    punk.first_track_previous_playlist();

    ASSERT_EQ("Black Parade", punk.playing_song());
    ASSERT_EQ("MCR", punk.playlist());

}

TEST_F(test_Jukebox, TestRepeater) {
    Jukebox punk;
    node* top = build_four_song_jukebox_helper("Black Parade", "Helena",
    "Everlong", "My Hero", "MCR", "Foo Fighters");
    punk.set_first(top);
    punk.set_current(top);

    ASSERT_FALSE(punk.juke_repeat("off"));
    ASSERT_TRUE(punk.juke_repeat("on"));

    punk.juke_repeat("on");
    punk.previous_song();

    ASSERT_EQ("My Hero", punk.playing_song());
    ASSERT_EQ("Foo Fighters", punk.playlist());

    punk.next_playlist();

    ASSERT_EQ("MCR", punk.playlist());

    punk.juke_repeat("off");
    punk.next_playlist();
    punk.next_playlist();

    ASSERT_EQ("Foo Fighters", punk.playlist());
}

TEST_F(test_Jukebox, TestPlaylistMove) {
    Jukebox punk;
    node* top = build_six_song_jukebox_helper("Black Parade", "Helena",
    "Everlong", "My Hero", "Come As You Are", "Heart-Shaped Box",
     "MCR", "Foo Fighters", "Nirvana");
    punk.set_first(top);
    punk.set_current(top);

    punk.move_playlist("MCR", "Foo Fighters", "Nirvana");
    // should now be Foo Fighters->MCR->Nirvana
    punk.juke_repeat("on");
    
    ASSERT_EQ("Black Parade", punk.playing_song());
    ASSERT_EQ("MCR", punk.playlist());

    punk.next_playlist();
    
    ASSERT_EQ("Nirvana", punk.playlist());
}