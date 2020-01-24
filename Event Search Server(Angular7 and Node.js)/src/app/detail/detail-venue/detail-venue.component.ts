import { Component, Input } from '@angular/core';

import { Venue } from '../venue-detail.model';
@Component({
    selector: 'app-detail-venue',
    templateUrl: './detail-venue.component.html'
})

export class DetailVenueComponent {
    @Input() venue: Venue = new Venue();
}
