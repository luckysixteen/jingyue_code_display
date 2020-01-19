export class Artist {
    constructor(
        public name: string = '',
        public isMusic: boolean = false,
        public followers: number = 0,
        public popularity: number = 0,
        public checkAtUrl: string = '',
        public imgUrl: Array<string> = []
    ) { }
}
