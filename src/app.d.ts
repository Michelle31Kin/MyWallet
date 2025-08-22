export {};

declare global {
	interface SaucerExposed {
		record_transaction:        (to_record: string) => Promise<string>;
		record_transfer:           (to_record: string) => Promise<string>;
		create_wallet:             (to_create: string) => Promise<string>;
		update_record:             (to_update: string) => Promise<string>;
		update_transfer:           (to_update: string) => Promise<string>;
		update_wallet:             (to_update: string) => Promise<string>;
		restore_wallet:			   (to_restore: string) => Promise<string>;
		get_wallets:               (criteria: string)  => Promise<string>;
		get_records:               (criteria: string)  => Promise<string>;
		delete_record:             (to_delete: string) => Promise<string>;
		delete_wallet:             (to_delete: string) => Promise<string>;
		delete_wallet_permanently: (to_delete: string) => Promise<string>;
	}

	interface Saucer {
		exposed: SaucerExposed;
	}

	interface Window {
		saucer: Saucer;
	}
}
