import { Component, Input } from '@angular/core';

import { EventDetail } from '../event-detail.model';

@Component({
    selector: 'app-detail-event',
    templateUrl: './detail-event.component.html',
    styleUrls: ['./detail-event.component.css']
})

export class DetailEventComponent {
    @Input() event: EventDetail = new EventDetail();
}

