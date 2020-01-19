import { Component, OnDestroy, ViewChild, ElementRef } from '@angular/core';
import { Results } from '../result.model';

import { SearchService } from '../../search.service';
import { viewAttached } from '@angular/core/src/render3/instructions';

@Component({
    selector: 'app-result-list',
    templateUrl: './result-list.component.html',
    styleUrls: ['./result-list.component.css']
})
export class ResultListComponent implements OnDestroy {
  results: Results[] = [];
  message = '';
  isSelected = false;
  element: HTMLElement;

  constructor(private searchResult: SearchService) {
    this.searchResult.resultListUpdated
      .subscribe((status: string) => {
        if (status === 'submitted') {
          this.message = this.searchResult.searchResult.message;
          this.results = this.searchResult.searchResult.list;
        }
      });
  }

  ngOnDestroy() {}

  onClickEvent(event: Results, index: number) {
    this.searchResult.getRestulDetail(event);
    this.isSelected = true;
    for (let j = 0; j < this.results.length; j++) {
      this.element = document.getElementById('result' + j) as HTMLElement;
      if (j === index) {
        this.element.classList.add('table-warning');
      } else {
        if (this.element.classList.length === 1) {
          this.element.classList.remove('table-warning');
        }
      }
    }
  }

  showDetail() {
    this.searchResult.isResultList.emit(false);
  }

  onFavorite(id: string) {
    this.searchResult.onFavorite(id);
    this.results = this.searchResult.searchResult.list;
  }

  unFavorite(id: string) {
    this.searchResult.unFavorite(id);
    this.results = this.searchResult.searchResult.list;
  }

  trimName(name: string) {
    const ln = 35;
    if (name.length > ln) {
      if (name.charAt(ln) === ' ') {
        let absName = name.slice(0, ln);
        absName = absName + ' ...';
        return absName;
      } else if (name.charAt(ln - 1) === ' ') {
        let absName = name.slice(0, ln);
        absName = absName + '...';
        return absName;
      } else {
        let absName = name.slice(0, name.lastIndexOf(' '));
        absName = absName + ' ...';
        return absName;
      }
    } else {return name; }
  }

}
