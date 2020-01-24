import { Component } from '@angular/core';

import { SearchService } from '../../search.service';

import { Detail } from '../detail.model';

@Component({
  selector: 'app-detail-head',
  templateUrl: './detail-head.component.html',
  styleUrls: ['./detail-head.component.css']
})

export class DetailHeadComponent {
  eventDetail: Detail = new Detail();
  twitterText: String = '';
  isLoaded = true;

  constructor(private detailSearch: SearchService) {
    this.detailSearch.detailUpdated
      .subscribe((message: string) => {
        if (message === 'updated') {
          this.eventDetail = this.detailSearch.eventDetail;
          if (this.isLoaded) {
            this.twitterText = 'Check out ' + this.eventDetail.eventDetail.name +
              ' located at ' + this.eventDetail.eventDetail.venueName +
              '. Website: ' + this.eventDetail.eventDetail.buyUrl;
          }
        }
      });
  }

  onReturnList() {
    this.detailSearch.isResultList.emit(true);
  }

  onFavorite(id: string) {
    this.detailSearch.onFavorite(id);
    this.eventDetail.eventDetail.isFavorite = true;
  }

  unFavorite(id: string) {
    this.detailSearch.unFavorite(id);
    this.eventDetail.eventDetail.isFavorite = false;
  }

}
