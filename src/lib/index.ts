// place files you want to import through the `$lib` alias in this folder.
// Will add the objects declarations and the function to be used in the html file here

//Wallet object
//Transaction object
//get entity function
//update entity function
//delete entity function
//create entity function
//search entity function
//perma_del function


export type Wallet = {
    id: number;
    name: string;
    currency: string;
    source: string;
    initial_amount: number;
    balance: number;
    color: string;
    created_at: string;
    updated_at: string;
    is_active: boolean;
};

export type Transaction = {
    id: number;
    wallet_name: string;
    type: string;
    category: string;
    description: string;
    amount: number;
    related_wallet_name: string;
    created_at: string;
    updated_at: string;
    is_archived: boolean;
}

export function create_wallet(to_create: Wallet) {
    let result;
    try {
        result = window.saucer.exposed.create_wallet(JSON.stringify(to_create));
    } catch (error) {
        result = "Failed to create wallet!!";
    }
    setTimeout(() => result = "", 3000);
}