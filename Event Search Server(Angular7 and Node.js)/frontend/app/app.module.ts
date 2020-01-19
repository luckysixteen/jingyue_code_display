import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';
import { AgmCoreModule } from '@agm/core';
import { MatTooltipModule } from '@angular/material/tooltip';
import { RoundProgressModule } from 'angular-svg-round-progressbar';

import { ReactiveFormsModule } from '@angular/forms';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { MatAutocompleteModule } from '@angular/material/autocomplete';

import { AppComponent } from './app.component';
import { ResultListComponent } from './list/result-list/result-list.component';
import { FavoriteListComponent } from './list/favorite-list/favorite-list.component';
import { DetailHeadComponent } from './detail/detail-head/detail-head.component';
import { DetailEventComponent } from './detail/detail-event/detail-event.component';
import { DetailArtistComponent } from './detail/detail-artist/detail-artist.component';
import { DetailVenueComponent } from './detail/detail-venue/detail-venue.component';
import { DetailUpcomingComponent } from './detail/detail-upcoming/detail-upcoming.component';

import { SearchService } from './search.service';
import { ResultSortPipe } from './result-sort.pipe';
import { UpcomingSortPipe } from './upcoming-sort.pipe';
import { from } from 'rxjs';

@NgModule({
  declarations: [
    AppComponent,
    ResultListComponent,
    FavoriteListComponent,
    DetailHeadComponent,
    DetailEventComponent,
    DetailArtistComponent,
    DetailVenueComponent,
    DetailUpcomingComponent,
    ResultSortPipe,
    UpcomingSortPipe
  ],
  imports: [
    BrowserModule,
    FormsModule,
    ReactiveFormsModule,
    HttpClientModule,
    AgmCoreModule.forRoot({
      apiKey: 'AIzaSyCUoNn_4e9fXWCyLvBKFg6oxsrlaLpGzeI'
    }),
    MatTooltipModule,
    RoundProgressModule,
    BrowserAnimationsModule,
    MatAutocompleteModule,
  ],
  providers: [SearchService],
  bootstrap: [AppComponent]
})
export class AppModule {}
