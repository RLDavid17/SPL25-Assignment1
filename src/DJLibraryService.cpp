#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}


DJLibraryService::~DJLibraryService() {
    for (AudioTrack* track : library) {
        delete track;
    }
    library.clear();
}

    /**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    
    std::cout << "TODO: Implement DJLibraryService::buildLibrary method\n"<< library_tracks.size() << " tracks to be loaded into library.\n";
    
    for (size_t i = 0; i < library_tracks.size(); i++) {
        
        auto track = library_tracks[i];
        AudioTrack* new_track = nullptr;

        if (track.type == "MP3") {
            bool has_id3_tags = track.extra_param2 != 0;
            new_track = new MP3Track(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, has_id3_tags);
            std::cout << "MP3Track created: " << track.title << " " << track.extra_param1 << "kbps" << std::endl;
        } 
        else
            if (track.type == "WAV") {
                new_track = new WAVTrack(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2);
                std::cout << "WAVTrack created: " << track.title << " " << track.extra_param1 << "Hz/" << track.extra_param2 << "bit" << std::endl;
        }

        if (new_track != nullptr) 
            library.push_back(new_track);
        
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    return playlist.find_track(track_title);
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, const std::vector<int>& track_indices) {
    // Your implementation here
    // For now, add a placeholder to fix the linker error
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
                                                
    playlist = Playlist(playlist_name);

    for (int index : track_indices) {
        int library_index = index - 1;

        if (library_index < 0 || library_index >= static_cast<int>(library.size())) {
            std::cout << "[WARNING] Invalid track index: " << index << std::endl;
            continue;
        }

        AudioTrack* canonical_track = library[library_index];
        PointerWrapper<AudioTrack> cloned_wrapper = canonical_track->clone();

        if (!cloned_wrapper) {
            std::cerr << "[ERROR] Failed to clone track: " << canonical_track->get_title() << std::endl;
            continue;
        }

        AudioTrack* raw_clone = cloned_wrapper.release();
        raw_clone->load();
        raw_clone->analyze_beatgrid();

        playlist.add_track(raw_clone);
        std::cout << "Added '" << raw_clone->get_title() << "' to playlist '" << playlist_name << "'" << std::endl;
    }

    std::cout << "[INFO] Playlist loaded: " << playlist_name << " (" << playlist.get_track_count() << " tracks)" << std::endl;

}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here

    std::vector<std::string> titles;

    std::vector<AudioTrack*> tracks = playlist.getTracks();    
    for (AudioTrack* track : tracks) {
        if (track != nullptr) {
            titles.push_back(track->get_title());
        }
    }
    
    return titles;
}
