export {};

declare global {
	interface SaucerExposed {
		____: (book: string) => Promise<boolean>;
		____: (book: string) => Promise<boolean>;
		____: (book: string) => Promise<boolean>;
		____: () => Promise<string>;
	}

	interface Saucer {
		exposed: SaucerExposed;
	}

	interface Window {
		saucer: Saucer;
	}
}
