const button = document.getElementById("refresh");

button.addEventListener('click',async()=>{
    const result = await fetch('/more');
    console.log(result);
    location.reload();
})