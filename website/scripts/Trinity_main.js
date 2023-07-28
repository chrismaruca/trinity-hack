let position = document.getElementById("pos");
let gas = document.getElementById("gas");
let gas_box = document.getElementById("gas-box");

let i = 0;
gas.onclick = () => {
    gas.textContent = i;
    i++;
}

const observer = new MutationObserver((mutations) => {
    for (mutation of mutations) {
        console.log(mutation.type);
        if (mutation.type === "childList") {
            if (gas.textContent >= 750) {
                gas_box.style.backgroundColor = "lightcoral";
                console.log("orangered");
            }
            else if (gas.textContent >= 250) {
                gas_box.style.backgroundColor = "orange";
                console.log("orange");
            }
            else {
                gas_box.style.backgroundColor = "lightgreen";
                console.log("lightgreen");
            }
        }
        
    }
})

observer.observe(gas, {attributes: true, characterData: true, subtree: true, childList: true});