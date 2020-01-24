import { EventDetail } from './event-detail.model';
import { Artist } from './artist-detail.model';
import { Venue } from './venue-detail.model';
import { UpcomingEvent } from './upcoming-event.model';

export class Detail {
    constructor(
        public eventDetail: EventDetail = new EventDetail(),
        public artists: Array<Artist> = [],
        public venue: Venue = new Venue(),
        public upcomingEvents: Array<UpcomingEvent> = []
    ) {}
}
