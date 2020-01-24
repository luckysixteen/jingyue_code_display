import { Component, ViewChild, OnInit } from '@angular/core';
import { NgForm, FormControl } from '@angular/forms';
import { HttpClient } from '@angular/common/http';
// for autocomplete
import { Observable } from 'rxjs';
import { map, startWith } from 'rxjs/operators';

import { SearchService } from './search.service';
import '../../node_modules/ngeohash/main.js';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})

export class AppComponent implements OnInit {

  @ViewChild('f') searchForm: NgForm;

  constructor(
    private httpClient: HttpClient,
    private searchService: SearchService) {
    this.ObtainLocation();
    this.searchService.isResultList
      .subscribe((isList: boolean) => {
        this.isResultList = isList;
      });
  }

  myControl = new FormControl();
  options: string[] = ['One', 'Two', 'Three'];
  filteredOptions: Observable<string[]>;

  defaultCategory = 'all';
  distance = '10';
  defaultUnit = 'miles';
  defaultLocation = 'location1';
  geoCode = [];

  submitted = false;
  isObtained = false;
  isResultList = true;

  ObtainLocation() {
    this.httpClient.get('http://ip-api.com/json', {
      observe: 'response',
      responseType: 'text'
    }).subscribe(
        data => {
          if (data['status'] === 200) {
            this.geoCode = [JSON.parse(data['body'])['lat'], JSON.parse(data['body'])['lon']];
            this.isObtained = true;
          }
        }
      );
  }

  onSubmit() {
    this.submitted = true;
    this.isResultList = true;
    if (this.searchForm.value.distance === '') {
      this.searchForm.value.distance = this.distance;
    }

    if (this.searchForm.valid) {
      this.searchService.getResultList(this.searchForm, this.geoCode);
      this.submitted = false;
    }
  }

  clearAll() {
    this.submitted = false;
    this.isResultList = true;

    localStorage.clear();
    this.searchForm.reset();
    this.searchForm.setValue({
      keyword: '',
      category: 'all',
      distance: '10',
      unit: 'miles',
      location: 'location1',
      locationInput: ''
    });

    // this.searchService.clearData();
  }

  // for autocomplete
  ngOnInit() {
    this.filteredOptions = this.myControl.valueChanges
      .pipe(
        startWith(''),
        map(value => this._filter(value))
      );
  }

  private _filter(value: string): string[] {
    const filterValue = value.toLowerCase();

    return this.options.filter(option => option.toLowerCase().includes(filterValue));
  }
}
