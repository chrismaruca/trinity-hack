
let dist = document.getElementById("dist");
let dist_inches = document.getElementById("dist-in");
let gas = document.getElementById("gas");
let gas_box = document.getElementById("gas-box");

const observer = new MutationObserver((mutations) => {
    for (mutation of mutations) {
        if (mutation.type === "childList") {
            if (gas.textContent >= 750) {
                gas_box.style.backgroundColor = "lightcoral";
            }
            else if (gas.textContent >= 250) {
                gas_box.style.backgroundColor = "orange";
            }
            else {
                gas_box.style.backgroundColor = "lightgreen";
            }

            dist_inches_inches.textContent = Math.round(dist_inches.textContent / 0.0254) / 100;
        }
    }
})

observer.observe(gas, {attributes: true, characterData: true, subtree: true, childList: true});