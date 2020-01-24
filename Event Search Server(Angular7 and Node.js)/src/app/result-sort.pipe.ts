import { Pipe, PipeTransform } from '@angular/core';
import { Results } from './list/result.model';

@Pipe({
  name: 'ResultSort'
})
export class ResultSortPipe implements PipeTransform {
  transform(results: Array<Results>): Array<Results> {
    if (!results) {
      return results;
    }

    return results.sort((a: Results, b: Results) => {
      if (a.date < b.date) {
        return -1;
      } else if (a.date > b.date) {
        return 1;
      } else {
        return 0;
      }
      //  return a > b;
    });
  }
}
