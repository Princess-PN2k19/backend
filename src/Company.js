import './App.css';

function Companies(props) {
    const { companies, companyName, inputHandler, addInputCompany } = props;
    return (
        <div className="company">
            <input name="companyName" type="text" onChange={inputHandler} placeholder="Enter a company" required>
            </input>
            <button disabled={!companyName} onClick={addInputCompany}>Add</button>
            <br/><br/>
            <table>
                <thead>
                    <tr>
                        <th>Company Id</th>
                        <th>Company Name</th>
                    </tr>
                </thead>
                <tbody>
                    {companies.map((item, index) => (
                        <tr key={index}>
                            <td>{item.company_id}</td>
                            <td>{item.company_name}</td>
                        </tr>
                    ))}
                </tbody>
            </table>
        </div>
    );
}

export default Companies;