import { Component } from '@angular/core';

import { SearchService } from '../../search.service';
import { Results } from '../result.model';

@Component({
  selector: 'app-favorite-list',
  templateUrl: './favorite-list.component.html'
})
export class FavoriteListComponent {
  results: Array<Results> = [];
  isSelected: Boolean = false;
  element: HTMLElement;

  constructor(private searchResult: SearchService) {
    this.searchResult.favoriteUpdated
      .subscribe((status: string) => {
        if (status === 'updated') {
          // for (let l = 0; l < localStorage.length; l++) {
          //   try {
          //   } catch {}
          // }
          console.log(localStorage);
        }
      });
  }

  showDetail() {}

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

  unFavorite(id: string) {}

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
    } else { return name; }
  }
}
