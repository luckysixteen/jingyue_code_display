export class Results {
    constructor(
        public date: string = 'N/A',
        public name: string = '',
        public eventId: string = '',
        public category: Array<string> = [],
        public venueName: string = 'N/A',
        public isFavorite: boolean = false) {}
}
