import { Component, Input } from '@angular/core';

import { Artist } from '../artist-detail.model';

@Component({
  selector: 'app-detail-artist',
  templateUrl: './detail-artist.component.html',
  styleUrls: ['./detail-artist.component.css']
})

export class DetailArtistComponent {
  @Input() artists: Array<Artist> = [];
}

