<script lang="ts">
	import type { Wallet, Transaction } from "$lib/index";
	import {
		create_wallet,
		record_transaction,
		record_transfer,
		update_record,
		update_wallet,
		update_transfer,
		get_wallets,
		get_records,
		delete_record,
		delete_wallet,
		trash_wallet,
		restore_wallet,
		refresh,
	} from "$lib/index";
	import { onMount } from "svelte";

	let a_record: Transaction;
	let a_wallet: Wallet;
	let success_popup: string = $state("");
	let error_popup: string = $state("");
	let fetched_wallets: Wallet[] = $state([]);
	let fetched_transactions: Transaction[] = $state([]);

	onMount(async () => refresh_wrap());
	a_wallet = {
		id: 0,
		name: "Frontend wallet",
		currency: "USD",
		source: "MOMO",
		initial_amount: 1000,
		balance: 800,
		color: "#fffff",
		created_at: "",
		updated_at: "",
		is_active: true,
	};
	a_record = {
		id: 0,
		wallet_name: "Frontend wallet",
		type: "income",
		category: "transportation",
		description: "just to try",
		amount: 200,
		related_wallet_name: "",
		created_at: "",
		updated_at: "",
		is_archived: false,
	};
	function fade_alert() {
		setTimeout(() => {
			success_popup = "";
			error_popup = "";
		}, 3000);
	}
	async function refresh_wrap() {
		const { wallets, records } = await refresh();
		fetched_wallets = wallets;
		fetched_transactions = records;
	}
	async function create_wallet_wrap(to_create: Wallet) {
		({ success_popup, error_popup } = await create_wallet(to_create));
		await refresh_wrap();
		fade_alert();
	}
	async function record_transaction_wrap(to_record: Transaction) {
		({ success_popup, error_popup } = await record_transaction(to_record));
		await refresh_wrap();
		fade_alert();
	}
	async function record_transfer_wrap(to_record: Transaction) {
		({ success_popup, error_popup } = await record_transfer(to_record));
		await refresh_wrap();
		fade_alert();
	}
	async function update_transaction_wrap(to_update: Transaction) {
		({ success_popup, error_popup } = await update_record(to_update));
		await refresh_wrap();
		fade_alert();
	}
	async function update_transfer_wrap(to_update: Transaction) {
		({ success_popup, error_popup } = await update_transfer(to_update));
		await refresh_wrap();
		fade_alert();
	}
	async function update_wallet_wrap(to_update: Wallet) {
		({ success_popup, error_popup } = await update_wallet(to_update));
		await refresh_wrap();
		fade_alert();
	}
	async function restore_wallet_wrap(to_restore: Wallet) {
		({ success_popup, error_popup } = await restore_wallet(to_restore));
		await refresh_wrap();
		fade_alert();
	}
	async function get_wallets_wrap() {
		fetched_wallets = await get_wallets();
		if (fetched_wallets.length === 0) error_popup = "No wallets found";
	}
	async function get_records_wrap() {
		fetched_transactions = await get_records();
		if (fetched_transactions.length === 0)
			error_popup = "No transactions found";
	}
	async function trash_wallet_wrap(to_trash: Wallet) {
		({ success_popup, error_popup } = await trash_wallet(to_trash));
		await refresh_wrap();
		fade_alert();
	}
	async function delete_wallet_wrap(to_delete: Wallet) {
		({ success_popup, error_popup } = await delete_wallet(to_delete));
		await refresh_wrap();
		fade_alert();
	}
	async function delete_record_wrap(to_delete: Transaction) {
		({ success_popup, error_popup } = await delete_record(to_delete));
		await refresh_wrap();
		fade_alert();
	}
// NEW CRUD FUNCTIONS FOR TRANSACTIONS

	// Start editing a transaction
	function start_edit_transaction(transaction: Transaction) {
		editing_transaction_id = transaction.id;
		editing_transaction = { ...transaction };
	}

	// Cancel editing
	function cancel_edit_transaction() {
		editing_transaction_id = null;
		editing_transaction = null;
	}

	// Save edited transaction
	async function save_edit_transaction() {
		if (editing_transaction) {
			if (editing_transaction.type === "transfer") {
				await update_transfer_wrap(editing_transaction);
			} else {
				await update_transaction_wrap(editing_transaction);
			}
			editing_transaction_id = null;
			editing_transaction = null;
		}
	}

	// Delete transaction with confirmation
	async function delete_transaction_confirm(transaction: Transaction) {
		if (confirm("Are you sure you want to delete this transaction?")) {
			await delete_record_wrap(transaction);
		}
	}

	// Create new transaction
	async function create_new_transaction() {
		if (new_transaction.wallet_name && new_transaction.category && new_transaction.description) {
			const transaction_to_create: Transaction = {
				id: Math.max(...fetched_transactions.map(t => t.id), 0) + 1,
				wallet_name: new_transaction.wallet_name!,
				type: new_transaction.type!,
				category: new_transaction.category!,
				description: new_transaction.description!,
				amount: new_transaction.amount!,
				related_wallet_name: new_transaction.related_wallet_name || "",
				created_at: new Date().toISOString(),
				updated_at: new Date().toISOString(),
				is_archived: new_transaction.is_archived!,
			};

			if (transaction_to_create.type === "transfer") {
				await record_transfer_wrap(transaction_to_create);
			} else {
				await record_transaction_wrap(transaction_to_create);
			}

			// Reset form
			new_transaction = {
				wallet_name: "",
				type: "expense",
				category: "",
				description: "",
				amount: 0,
				related_wallet_name: "",
				is_archived: false,
			};
			is_creating = false;
		} else {
			error_popup = "Please fill in all required fields";
			fade_alert();
		}
	}

	// Utility functions
	function format_date(date_string: string): string {
		return new Date(date_string).toLocaleDateString();
	}

	function format_amount(amount: number, type: string): string {
		const formatted = amount.toFixed(2);
		if (type === "expense") return `-$${formatted}`;
		if (type === "income") return `+$${formatted}`;
		return `$${formatted}`;
	}

	function get_type_color(type: string): string {
		switch (type) {
			case "expense": return "text-red-600";
			case "income": return "text-green-600";
			case "transfer": return "text-blue-600";
			default: return "text-gray-600";
		}
	}
</script>

<!-- Success/Error Alerts -->
{#if success_popup}
	<div class="fixed top-4 right-4 bg-green-500 text-white px-4 py-2 rounded-lg shadow-lg z-50">
		{success_popup}
	</div>
{/if}

{#if error_popup}
	<div class="fixed top-4 right-4 bg-red-500 text-white px-4 py-2 rounded-lg shadow-lg z-50">
		{error_popup}
	</div>
{/if}

<div class="p-6 max-w-7xl mx-auto">
	<div class="flex justify-between items-center mb-6">
		<h1 class="text-3xl font-bold text-gray-800">Transaction Management</h1>
		<button
			onclick={() => (is_creating = true)}
			class="flex items-center gap-2 bg-blue-600 text-white px-4 py-2 rounded-lg hover:bg-blue-700 transition-colors"
		>
			<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
				<path d="M5 12h14"></path>
				<path d="M12 5v14"></path>
			</svg>
			Add Transaction
		</button>
	</div>

	<!-- Create new transaction form -->
	{#if is_creating}
		<div class="mb-6 bg-gray-50 p-6 rounded-lg border shadow-sm">
			<h3 class="text-lg font-semibold mb-4">Create New Transaction</h3>
			<div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4 mb-4">
				<div>
					<label class="block text-sm font-medium text-gray-700 mb-1">Wallet Name</label>
					<select
						bind:value={new_transaction.wallet_name}
						class="w-full px-3 py-2 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
					>
						<option value="">Select Wallet</option>
						{#each fetched_wallets as wallet}
							<option value={wallet.name}>{wallet.name}</option>
						{/each}
					</select>
				</div>
				
				<div>
					<label class="block text-sm font-medium text-gray-700 mb-1">Type</label>
					<select
						bind:value={new_transaction.type}
						class="w-full px-3 py-2 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
					>
						<option value="expense">Expense</option>
						<option value="income">Income</option>
						<option value="transfer">Transfer</option>
					</select>
				</div>
				
				<div>
					<label class="block text-sm font-medium text-gray-700 mb-1">Category</label>
					<input
						type="text"
						bind:value={new_transaction.category}
						placeholder="Enter category"
						class="w-full px-3 py-2 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
					/>
				</div>
				
				<div>
					<label class="block text-sm font-medium text-gray-700 mb-1">Description</label>
					<input
						type="text"
						bind:value={new_transaction.description}
						placeholder="Enter description"
						class="w-full px-3 py-2 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
					/>
				</div>
				
				<div>
					<label class="block text-sm font-medium text-gray-700 mb-1">Amount</label>
					<input
						type="number"
						step="0.01"
						bind:value={new_transaction.amount}
						placeholder="0.00"
						class="w-full px-3 py-2 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
					/>
				</div>
				
				{#if new_transaction.type === "transfer"}
					<div>
						<label class="block text-sm font-medium text-gray-700 mb-1">Related Wallet</label>
						<select
							bind:value={new_transaction.related_wallet_name}
							class="w-full px-3 py-2 border rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
						>
							<option value="">Select Related Wallet</option>
							{#each fetched_wallets as wallet}
								<option value={wallet.name}>{wallet.name}</option>
							{/each}
						</select>
					</div>
				{/if}
			</div>
			
			<div class="flex gap-2">
				<button
					onclick={create_new_transaction}
					class="flex items-center gap-2 bg-green-600 text-white px-4 py-2 rounded-md hover:bg-green-700 transition-colors"
				>
					<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
						<path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"></path>
						<polyline points="17,21 17,13 7,13 7,21"></polyline>
						<polyline points="7,3 7,8 15,8"></polyline>
					</svg>
					Create
				</button>
				<button
					onclick={() => (is_creating = false)}
					class="flex items-center gap-2 bg-gray-500 text-white px-4 py-2 rounded-md hover:bg-gray-600 transition-colors"
				>
					<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
						<path d="M18 6L6 18"></path>
						<path d="M6 6l12 12"></path>
					</svg>
					Cancel
				</button>
			</div>
		</div>
	{/if}

	<!-- Transactions Table -->
	<div class="bg-white rounded-lg shadow-md overflow-hidden">
		<div class="overflow-x-auto">
			<table class="w-full">
				<thead class="bg-gray-50">
					<tr>
						<th class="px-4 py-3 text-left text-sm font-medium text-gray-700">Date</th>
						<th class="px-4 py-3 text-left text-sm font-medium text-gray-700">Wallet</th>
						<th class="px-4 py-3 text-left text-sm font-medium text-gray-700">Type</th>
						<th class="px-4 py-3 text-left text-sm font-medium text-gray-700">Category</th>
						<th class="px-4 py-3 text-left text-sm font-medium text-gray-700">Description</th>
						<th class="px-4 py-3 text-left text-sm font-medium text-gray-700">Amount</th>
						<th class="px-4 py-3 text-left text-sm font-medium text-gray-700">Actions</th>
					</tr>
				</thead>
				<tbody class="divide-y divide-gray-200">
					{#each fetched_transactions as transaction (transaction.id)}
						<tr class="hover:bg-gray-50 transition-colors">
							{#if editing_transaction_id === transaction.id && editing_transaction}
								<!-- Edit Mode Row -->
								<td class="px-4 py-3 text-sm text-gray-600">
									{format_date(transaction.created_at)}
								</td>
								<td class="px-4 py-3">
									<select
										bind:value={editing_transaction.wallet_name}
										class="w-full px-2 py-1 text-sm border rounded focus:outline-none focus:ring-2 focus:ring-blue-500"
									>
										{#each fetched_wallets as wallet}
											<option value={wallet.name}>{wallet.name}</option>
										{/each}
									</select>
								</td>
								<td class="px-4 py-3">
									<select
										bind:value={editing_transaction.type}
										class="w-full px-2 py-1 text-sm border rounded focus:outline-none focus:ring-2 focus:ring-blue-500"
									>
										<option value="expense">Expense</option>
										<option value="income">Income</option>
										<option value="transfer">Transfer</option>
									</select>
								</td>
								<td class="px-4 py-3">
									<input
										type="text"
										bind:value={editing_transaction.category}
										class="w-full px-2 py-1 text-sm border rounded focus:outline-none focus:ring-2 focus:ring-blue-500"
									/>
								</td>
								<td class="px-4 py-3">
									<input
										type="text"
										bind:value={editing_transaction.description}
										class="w-full px-2 py-1 text-sm border rounded focus:outline-none focus:ring-2 focus:ring-blue-500"
									/>
								</td>
								<td class="px-4 py-3">
									<input
										type="number"
										step="0.01"
										bind:value={editing_transaction.amount}
										class="w-full px-2 py-1 text-sm border rounded focus:outline-none focus:ring-2 focus:ring-blue-500"
									/>
								</td>
								<td class="px-4 py-3">
									<div class="flex gap-1">
										<button
											onclick={save_edit_transaction}
											class="p-1 text-green-600 hover:bg-green-100 rounded transition-colors"
											title="Save"
										>
											<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
												<path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"></path>
												<polyline points="17,21 17,13 7,13 7,21"></polyline>
												<polyline points="7,3 7,8 15,8"></polyline>
											</svg>
										</button>
										<button
											onclick={cancel_edit_transaction}
											class="p-1 text-gray-600 hover:bg-gray-100 rounded transition-colors"
											title="Cancel"
										>
											<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
												<path d="M18 6L6 18"></path>
												<path d="M6 6l12 12"></path>
											</svg>
										</button>
									</div>
								</td>
							{:else}
								<!-- View Mode Row -->
								<td class="px-4 py-3 text-sm text-gray-600">
									{format_date(transaction.created_at)}
								</td>
								<td class="px-4 py-3 text-sm font-medium text-gray-900">
									{transaction.wallet_name}
								</td>
								<td class="px-4 py-3">
									<span class="text-sm font-medium capitalize {get_type_color(transaction.type)}">
										{transaction.type}
									</span>
								</td>
								<td class="px-4 py-3 text-sm text-gray-600">
									{transaction.category}
								</td>
								<td class="px-4 py-3 text-sm text-gray-600">
									{transaction.description}
								</td>
								<td class="px-4 py-3 text-sm font-medium">
									<span class={get_type_color(transaction.type)}>
										{format_amount(transaction.amount, transaction.type)}
									</span>
								</td>
								<td class="px-4 py-3">
									<div class="flex gap-1">
										<button
											onclick={() => start_edit_transaction(transaction)}
											class="p-1 text-blue-600 hover:bg-blue-100 rounded transition-colors"
											title="Edit Transaction"
										>
											<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
												<path d="M11 4H4a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-7"></path>
												<path d="M18.5 2.5a2.121 2.121 0 0 1 3 3L12 15l-4 1 1-4 9.5-9.5z"></path>
											</svg>
										</button>
										<button
											onclick={() => delete_transaction_confirm(transaction)}
											class="p-1 text-red-600 hover:bg-red-100 rounded transition-colors"
											title="Delete Transaction"
										>
											<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
												<polyline points="3,6 5,6 21,6"></polyline>
												<path d="M19,6v14a2,2,0,0,1-2,2H7a2,2,0,0,1-2-2V6m3,0V4a2,2,0,0,1,2-2h4a2,2,0,0,1,2,2V6"></path>
											</svg>
										</button>
									</div>
								</td>
							{/if}
						</tr>
					{/each}
				</tbody>
			</table>
		</div>
	</div>

	<!-- Empty state -->
	{#if fetched_transactions.length === 0}
		<div class="text-center py-12">
			<svg xmlns="http://www.w3.org/2000/svg" width="48" height="48" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1" stroke-linecap="round" stroke-linejoin="round" class="mx-auto text-gray-400 mb-4">
				<path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"></path>
				<polyline points="14,2 14,8 20,8"></polyline>
				<line x1="16" y1="13" x2="8" y2="13"></line>
				<line x1="16" y1="17" x2="8" y2="17"></line>
				<polyline points="10,9 9,9 8,9"></polyline>
			</svg>
			<p class="text-gray-500 text-lg">No transactions found</p>
			<p class="text-gray-400 text-sm">Click "Add Transaction" to create your first transaction.</p>
		</div>
	{/if}
</div>
</script>

<style>
	/* Custom scrollbar for table */
	.overflow-x-auto::-webkit-scrollbar {
		height: 8px;
	}
	
	.overflow-x-auto::-webkit-scrollbar-track {
		background: #f1f1f1;
		border-radius: 4px;
	}
	
	.overflow-x-auto::-webkit-scrollbar-thumb {
		background: #c1c1c1;
		border-radius: 4px;
	}
	
	.overflow-x-auto::-webkit-scrollbar-thumb:hover {
		background: #a8a8a8;
	}
</style>

<main class="bg-base-100 text-base-content">
    <p class = "font-extrabold justify-center mt-6 border rounded-2xl text-6xl p-64">This is the all transactions page</p>
</main>
