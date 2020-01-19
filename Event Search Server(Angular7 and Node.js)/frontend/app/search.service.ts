import { Injectable, EventEmitter } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import { Results } from './list/result.model';
import { Detail } from './detail/detail.model';
import { EventDetail } from './detail/event-detail.model';
import { Artist } from './detail/artist-detail.model';
import { Venue } from './detail/venue-detail.model';
import { UpcomingEvent } from './detail/upcoming-event.model';

@Injectable({
    providedIn: 'root',
})
export class SearchService {
    searchData = {
        keyword: '',
        category: '',
        distance: '',
        unit: '',
        geoCode: []
    };
    searchResult: {message: string, list: Array<Results>} = {
        message: '',
        list: []
    };
    favoriteResult: Array<Results> = [];

    eventDetail: Detail = new Detail();

    isResultList = new EventEmitter<boolean>();
    resultListUpdated = new EventEmitter<string>();
    detailUpdated = new EventEmitter<string>();
    favoriteUpdated = new EventEmitter<string>();

    constructor(private http: HttpClient) {}

    getResultList(searchForm, geoCode: any[]) {
        this.clearData();

        this.searchData.keyword = searchForm.value.keyword.replace(/ /g, '+');
        this.searchData.category = searchForm.value.category;
        this.searchData.distance = searchForm.value.distance;
        this.searchData.unit = searchForm.value.unit;
        if (searchForm.value.location === 'location1') {
            this.searchData.geoCode = geoCode;
        } else {
            this.searchData.geoCode = searchForm.value.locationInput;
        }

        this.http.get('http://localhost:3000/searchForm', { params: this.searchData })
            .subscribe((responseData) => {
                if (responseData['message'] === 'success') {
                    this.processResultList(responseData['result']);
                    this.resultListUpdated.emit('submitted');
                } else {
                    this.searchResult.message = 'fail';
                }
            });
    }

    processResultList(responseData) {
        if (responseData['page']['totalElements'] === 0) {
            this.searchResult.message = 'none';
        } else {
            this.searchResult.message = 'done';
            for (let i = 0; i < responseData['_embedded']['events'].length; i++ ) {
                const resultJSON = responseData['_embedded']['events'][i];
                const result = new Results();
                try {
                    result.date = resultJSON['dates']['start']['localDate'];
                } catch {
                    result.date = 'N/A';
                }
                try {
                    result.name = resultJSON['name'];
                } catch {
                    result.date = 'N/A';
                }
                try {
                    result.eventId = resultJSON['id'];
                } catch {
                    result.date = 'N/A';
                }
                try {
                    result.category.push(resultJSON['classifications'][0]['genre']['name']);
                } catch {}
                try {
                    result.category.push(resultJSON['classifications'][0]['segment']['name']);
                } catch {}
                try {
                    result.venueName = resultJSON['_embedded']['venues'][0]['name'];
                } catch {
                    result.date = 'N/A';
                }
                this.searchResult.list.push(result);
            }
        }
    }

    getRestulDetail(event: Results) {
        const query = {id: event.eventId};
        this.http.get('http://localhost:3000/searchDetail', { params: query })
            .subscribe((responseData) => {
                if (responseData['message'] === 'success') {
                    this.eventDetail = new Detail();
                    this.processEventDetail(responseData['result']);
                    this.eventDetail.eventDetail.isFavorite = event.isFavorite;
                    const venueQuery = { name: this.eventDetail.eventDetail.venueName };
                    // search artists detail
                    if (this.eventDetail.eventDetail.artists !== []) {
                        for (let a = 0; a < this.eventDetail.eventDetail.artists.length; a++) {
                            const artistsQuery = {
                                artist: this.eventDetail.eventDetail.artists[a]
                            };
                            // spotify+google
                            if (this.eventDetail.eventDetail.musicArtists.includes(this.eventDetail.eventDetail.artists[a])) {
                                this.http.get('http://localhost:3000/searchSpotifyImg', { params: artistsQuery })
                                    .subscribe((responseArtist) => {
                                        // console.log(responseArtist);
                                        this.processArtistDetail(responseArtist, this.eventDetail.eventDetail.artists[a], 1);
                                    });
                            } else {
                                // only google
                                this.http.get('http://localhost:3000/searchArtistImg', { params: artistsQuery })
                                    .subscribe((responseArtist) => {
                                        this.processArtistDetail(responseArtist, this.eventDetail.eventDetail.artists[a], 0);
                                    });
                            }
                        }
                    }
                    // search venue and upcoming events
                    if (venueQuery.name !== '') {
                        this.http.get('http://localhost:3000/searchVenue', { params: venueQuery })
                            .subscribe((responseVenue) => {
                                this.processVenueDetail(responseVenue);
                            });
                        this.http.get('http://localhost:3000/searchUpcoming', { params: venueQuery })
                            .subscribe((responseUpcoming) => {
                                if (responseUpcoming['message'] === 'success') {
                                    this.processUpcoming(responseUpcoming);
                                }
                            });
                    }
                    this.detailUpdated.emit('updated');
                }
            });
        // console.log('serveice: ', this.eventDetail.details);
        this.isResultList.emit(false);
    }

    processEventDetail(responseData) {
        const event = new EventDetail();
        event.name = responseData['name'];
        event.eventId = responseData.id;
        try {
            const num = responseData['_embedded']['attractions'].length;
            for (let art = 0; art < num; art++) {
                event.artists.push(responseData['_embedded']['attractions'][art]['name']);
                try {
                    if (responseData['_embedded']['attractions'][art]['classifications'][0]['segment']['name'] === 'Music') {
                        event.musicArtists.push(responseData['_embedded']['attractions'][art]['name']);
                    }
                } catch {}
            }
        } catch {}
        try {
            event.venueName = responseData['_embedded']['venues'][0]['name'];
        } catch {}
        try {
            event.date = responseData['dates']['start']['localDate'];
        } catch {}
        try {
            event.time = responseData['dates']['start']['localTime'];
        } catch {}
        try {
            event.category.push(responseData['classifications'][0]['segment']['name']);
        } catch {}
        try {
            event.category.push(responseData['classifications'][0]['genre']['name']);
        } catch {}
        try {
            event.currency = responseData['priceRanges'][0]['currency'];
        } catch {}
        try {
            event.priceRange.push(responseData['priceRanges'][0]['min']);
        } catch {}
        try {
            event.priceRange.push(responseData['priceRanges'][0]['max']);
        } catch {}
        try {
            event.ticketStatus = responseData['dates']['status']['code'];
        } catch {}
        try {
            event.buyUrl = responseData['url'];
        } catch {}
        try {
            event.seatMapUrl = responseData['seatmap']['staticUrl'];
        } catch {}
        this.eventDetail.eventDetail = event;
    }

    processArtistDetail(responseData, artName: string, opt: number) {
        const artistImgData = responseData.img;
        const artist = new Artist();
        let hasSpotify = false;
        if (opt === 1) {
            try {
                if (responseData.spotify.artists.items.length !== 0) {
                    hasSpotify = true;
                }
            } catch {}
            // do spotify
            if (hasSpotify) {
                const artistSpotifyData = responseData.spotify.artists.items;
                for (let s = 0; s < artistSpotifyData.length; s++) {
                    if (artistSpotifyData[s].name.toUpperCase() === artName.toUpperCase()) {
                      artist.name = artName;
                      artist.isMusic = true;
                      artist.followers = artistSpotifyData[s].followers.total;
                      artist.popularity = artistSpotifyData[s].popularity;
                      artist.checkAtUrl = artistSpotifyData[s].external_urls.spotify;
                      break;
                    }
                }
            }
        }
        // do google search
        try {
            if (artist.name === '') {
                artist.name = artistImgData.queries.request[0].searchTerms;
            }
        } catch {}
        try {
            if (artistImgData.items.length > 0) {
                for (let j = 0; j < artistImgData.items.length; j++) {
                    try {
                        artist.imgUrl.push(artistImgData.items[j].link);
                    } catch { }
                }
            }
        } catch {}
        this.eventDetail.artists.push(artist);
        // console.log(this.eventDetail.artists);
    }

    processVenueDetail(responseData) {
        const venue = new Venue();
        const data = responseData['_embedded']['venues'][0];
        venue.name = data['name'];
        try {
            venue.address = data['address']['line1'];
        } catch {}
        try {
            venue.city = data['city']['name'];
        } catch {}
        try {
            venue.state = data['state']['name'];
        } catch {}
        try {
            venue.phone = data['boxOfficeInfo']['phoneNumberDetail'];
        } catch {}
        try {
            venue.openHour = data['boxOfficeInfo']['openHoursDetail'];
        } catch {}
        try {
            venue.generalRule = data['generalInfo']['generalRule'];
        } catch {}
        try {
            venue.childRule = data['generalInfo']['childRule'];
        } catch {}
        try {
            venue.geo.push(Number(data['location']['latitude']));
            venue.geo.push(Number(data['location']['longitude']));
        } catch {}
        this.eventDetail.venue = venue;
    }

    processUpcoming(responseData) {
        if (responseData['result']['resultsPage']['totalEntries'] !== 0) {
            const listData = responseData['result']['resultsPage']['results']['event'];
            for (let i = 0; i < listData.length; i++) {
              const upcomingData = listData[i];
                const upcomings = new UpcomingEvent();
                try {
                    upcomings.name = upcomingData.displayName;
                } catch {}
                try {
                    upcomings.eventUrl = upcomingData.uri;
                } catch {}
                try {
                    upcomings.artist = upcomingData.performance[0].displayName;
                } catch {}
                try {
                    upcomings.date = upcomingData.start.date;
                } catch {}
                try {
                    upcomings.time = upcomingData.start.time;
                } catch {}
                try {
                    upcomings.type = upcomingData.type;
                } catch {}
                this.eventDetail.upcomingEvents.push(upcomings);
            }
        }
    }

    onFavorite(id: string) {
        for (let e = 0; e < this.searchResult.list.length; e++) {
            if (this.searchResult.list[e].eventId === id) {
                this.searchResult.list[e].isFavorite = true;
                try {
                    this.favoriteResult = JSON.parse(localStorage.favorite);
                } catch {}
                this.favoriteResult.push(this.searchResult.list[e]);
                localStorage.favorite = JSON.stringify(this.favoriteResult);
                break;
            }
        }
        if (this.eventDetail.eventDetail.eventId === id) {
            this.eventDetail.eventDetail.isFavorite = true;
        }
        this.favoriteUpdated.emit('updated');
    }

    unFavorite(id: string) {
        for (let e = 0; e < this.searchResult.list.length; e++) {
            if (this.searchResult.list[e].eventId === id) {
                this.searchResult.list[e].isFavorite = false;
                try {
                    this.favoriteResult = JSON.parse(localStorage.favorite);
                    for (let f = 0; f < this.favoriteResult.length; f++) {
                        if (this.favoriteResult[f].eventId === id) {
                            this.favoriteResult.splice(f, 1);
                        }
                    }
                } catch {
                    localStorage.clear();
                }
                break;
            }
        }
        if (this.eventDetail.eventDetail.eventId === id) {
            this.eventDetail.eventDetail.isFavorite = false;
        }
        this.favoriteUpdated.emit('updated');
    }

    clearData() {
        this.searchData = {
            keyword: '',
            category: '',
            distance: '',
            unit: '',
            geoCode: []
        };
        this.searchResult = {message: '', list: []};
        this.eventDetail = new Detail();
    }
}
