import { Component, Input, ViewChild, ElementRef } from '@angular/core';

import { UpcomingEvent } from '../upcoming-event.model';
import { ElementDef } from '@angular/core/src/view';

@Component({
    selector: 'app-detail-upcoming',
    templateUrl: './detail-upcoming.component.html'
})

export class DetailUpcomingComponent {
    @Input() upcomingEvents: Array<UpcomingEvent> = [];
    @ViewChild('sort') sortSelect: ElementRef;

    defaultPath = 'default';
    defaultOrder = 1;
    isDefault = true;
    isMore = false;


    sortChange() {
        if (this.sortSelect.nativeElement.value !== 'default') {
            this.isDefault = false;
        } else {
            this.isDefault = true;
        }
    }

    showMore() {
        this.isMore = true;
    }

    showLess() {
        this.isMore = false;
    }
}
