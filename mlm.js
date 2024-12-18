// MLM Data Structure
const distributors = new Map(); //distributors and their details
const hierarchy = new Map(); //parent-child relationships

// open form
function openForm(formId) {
    const forms = document.querySelectorAll('.form-content');
    forms.forEach(form => form.style.display = 'none');

    const tabs = document.querySelectorAll('.tab-link');
    tabs.forEach(tab => tab.classList.remove('active'));

    document.getElementById(formId).style.display = 'block';

    const activeTab = document.querySelector(`.tab-link[onclick="openForm('${formId}')"]`);
    activeTab.classList.add('active');

    clearInputs(formId);
}

// clear input
function clearInputs(formId) {
    const form = document.getElementById(formId);
    const inputs = form.querySelectorAll('input');
    inputs.forEach(input => input.value = '');
}

// add distributor
function addDistributor() {
    const id = document.getElementById('distributor-id').value;
    const name = document.getElementById('distributor-name').value;
    if (id && name) {
        if (!distributors.has(id)) {
            distributors.set(id, { name, sales: 0 });
            hierarchy.set(id, []);
            document.getElementById('output').innerHTML = `Added distributor: ${name} with ID: ${id}`;
        } else {
            alert('Distributor ID already exists!');
        }
        clearInputs('addDistributor');
    } else {
        alert('Please enter both ID and name');
    }
}

// add relationship
function addRelationship() {
    const parentId = document.getElementById('parent-id').value;
    const childId = document.getElementById('child-id').value;
    if (parentId && childId) {
        if (distributors.has(parentId) && distributors.has(childId)) {
            hierarchy.get(parentId).push(childId);
            document.getElementById('output').innerHTML = `Added relationship: Parent ID ${parentId} -> Child ID ${childId}`;
        } else {
            alert('Parent or child ID does not exist!');
        }
        clearInputs('addRelationship');
    } else {
        alert('Please enter both parent and child distributor IDs');
    }
}

//update sales
function updateSales() {
    const id = document.getElementById('sales-id').value;
    const sales = document.getElementById('sales-amount').value;
    if (id && sales) {
        if (distributors.has(id)) {
            distributors.get(id).sales += parseFloat(sales);
            document.getElementById('output').innerHTML = `Updated sales for distributor ID ${id}: INR ${sales}`;
        } else {
            alert('Distributor ID does not exist!');
        }
        clearInputs('updateSales');
    } else {
        alert('Please enter both ID and sales amount');
    }
}

//calculate commissions
function calculateCommissions() {
    if (distributors.size === 0) {
        document.getElementById('output').innerHTML = 'No distributors to calculate commissions for.';
        return;
    }

    let output = '<h3>Commission Calculations:</h3>';
    distributors.forEach((distributor, id) => {
        const baseCommission = distributor.sales * 0.1; // 10% commission
        const downlineSales = calculateDownlineSales(id);
        const additionalCommission = downlineSales * 0.05; // 5% from downline
        const totalCommission = baseCommission + additionalCommission;

        output += `Distributor ID ${id}: Base Commission: INR ${baseCommission.toFixed(2)}, Downline Sales: INR ${downlineSales.toFixed(2)}, Total Commission: INR ${totalCommission.toFixed(2)}<br>`;
    });

    document.getElementById('output').innerHTML = output;
}

// Helper calculate downline sales
function calculateDownlineSales(id) {
    let totalSales = 0;
    const stack = [...(hierarchy.get(id) || [])];

    while (stack.length) {
        const childId = stack.pop();
        if (distributors.has(childId)) {
            totalSales += distributors.get(childId).sales;
            stack.push(...(hierarchy.get(childId) || []));
        }
    }

    return totalSales;
}

//print the network
function printNetwork() {
    if (distributors.size === 0) {
        document.getElementById('output').innerHTML = 'No network to display.';
        return;
    }

    let output = '<h3>Distributor Network:</h3>';
    distributors.forEach((distributor, id) => {
        output += `Distributor ID ${id} (${distributor.name}): Children -> ${hierarchy.get(id).join(', ') || 'None'}<br>`;
    });

    document.getElementById('output').innerHTML = output;
}

//delete distributor
function deleteDistributor() {
    const id = document.getElementById('delete-id').value;
    if (id) {
        if (distributors.has(id)) {
            distributors.delete(id);
            hierarchy.delete(id);

            hierarchy.forEach(children => {
                const index = children.indexOf(id);
                if (index !== -1) children.splice(index, 1);
            });

            document.getElementById('output').innerHTML = `Deleted distributor with ID: ${id}`;
        } else {
            alert('Distributor ID does not exist!');
        }
        clearInputs('deleteDistributor');
    } else {
        alert('Please enter distributor ID to delete');
    }
}

//update relationship
function updateRelationship() {
    const oldParentId = document.getElementById('old-parent-id').value;
    const newParentId = document.getElementById('new-parent-id').value;
    const childId = document.getElementById('update-child-id').value;

    if (oldParentId && newParentId && childId) {
        if (hierarchy.has(oldParentId) && hierarchy.has(newParentId) && distributors.has(childId)) {
            const children = hierarchy.get(oldParentId);
            const index = children.indexOf(childId);

            if (index !== -1) {
                children.splice(index, 1);
                hierarchy.get(newParentId).push(childId);

                document.getElementById('output').innerHTML = `Updated relationship: Child ID ${childId} moved from Parent ID ${oldParentId} to Parent ID ${newParentId}`;
            } else {
                alert('Child ID not found under the specified old parent ID');
            }
        } else {
            alert('Old parent, new parent, or child ID does not exist!');
        }
        clearInputs('updateRelationship');
    } else {
        alert('Please enter all details');
    }
}
