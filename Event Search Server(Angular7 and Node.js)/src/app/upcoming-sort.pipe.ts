import { Pipe, PipeTransform } from '@angular/core';
import { UpcomingEvent } from './detail/upcoming-event.model';

@Pipe({
    name: 'UpcomingSort'
})
export class UpcomingSortPipe implements PipeTransform {
    transform(upcoming: Array<UpcomingEvent>, prop: string, order: number): Array<UpcomingEvent> {
        if (!upcoming || !prop || !order) {
            return upcoming;
        }

        return upcoming.sort((a: UpcomingEvent, b: UpcomingEvent) => {
            return a[prop] > b[prop] ? order : order * (- 1);
        });
    }
}
