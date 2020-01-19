export class Venue {
    constructor(
        public name: string = '',
        public geo: Array<number> = [],
        public address: string = '',
        public city: string = '',
        public state: string = '',
        public phone: string = '',
        public openHour: string = '',
        public generalRule: string = '',
        public childRule: string = ''
    ) { }
}
