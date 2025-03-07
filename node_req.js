
async function batch(){

    const url = "https://jsonplaceholder.typicode.com/posts";
    const headers = {"Content-Type" : "application/json"};
    let promises = [];
    for(let k = 0; k < 100; k++){
        const response = fetch(url, {
            headers: headers
        });
        promises.push(response)
    }

    const result = await Promise.all(promises);
    for(const res of result) {
        console.log(res);
    }
 
 }


 
 async function main(){
     
    await batch();     
     
}

main();
 