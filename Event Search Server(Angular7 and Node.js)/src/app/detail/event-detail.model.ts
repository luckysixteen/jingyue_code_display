export class EventDetail {
    constructor(
        public name: string = '',
        public eventId: string = '',
        public isFavorite: boolean = false,
        public artists: Array<string> = [],
        public musicArtists: Array<string> = [],
        public venueName: string = '',
        public date: string = '',
        public time: string = '',
        public category: Array<string> = [],
        public currency: string = '',
        public priceRange: Array<number> = [],
        public ticketStatus: string = '',
        public buyUrl: string = '',
        public seatMapUrl: string = ''
    ) { }
}
