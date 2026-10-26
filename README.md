# AuraStore — Full-Stack E-Commerce Platform

A complete, high-performance full-stack web application featuring a modern **HTML5 / CSS3 / JavaScript** frontend powered by a custom **C++ HTTP Server Backend** and **Persistent Database Engine**.

---

## 🏛️ System Architecture

```
ecommerce-storefront/
├── index.html            # Responsive frontend markup with live C++ backend status badge
├── styles.css            # Design tokens, CSS variables, dark/light themes, animations
├── app.js                # Frontend client engine (auto-connects to C++ REST API)
├── README.md             # Project documentation
│
└── backend/              # C++ Backend & Database Engine
    ├── main.cpp          # Server entrypoint & signal handling
    ├── http_server.hpp   # Multithreaded Winsock2 HTTP Server definition
    ├── http_server.cpp   # HTTP parsing, REST router, CORS headers & static asset server
    ├── database.hpp      # Persistent C++ Database interface
    ├── database.cpp      # ACID-style table persistence (JSON on disk with mutex protection)
    ├── models.hpp        # Data models (Product, Order, OrderItem, Coupon)
    ├── json_helper.hpp   # Header-only JSON serialization & parsing helpers
    ├── win_sync.hpp      # Windows native Mutex & LockGuard primitives
    ├── build.bat         # Automated compilation script (g++ / MinGW)
    ├── run.bat           # Run script (launches server on port 8080)
    │
    ├── bin/
    │   └── server.exe    # Compiled 32/64-bit native executable binary
    │
    └── data/             # Persistent Database Storage Directory
        ├── products.json # Products table (catalogue data)
        ├── orders.json   # Orders table (customer checkouts)
        └── coupons.json  # Coupons table (discounts & usage tracking)
```

---

## 🚀 How to Run

### Step 1: Start the C++ Backend Server
Navigate to the `backend` folder and run `run.bat` (or compile with `build.bat`):

```powershell
cd C:\Users\user\.gemini\antigravity\scratch\ecommerce-storefront\backend
.\run.bat
```

*This will automatically start the Winsock HTTP server on `http://localhost:8080`.*

### Step 2: Open the Frontend
Once the server is running, you can access the full website in either of two ways:

1. **Directly through the C++ Server**:
   Navigate to [**`http://localhost:8080`**](http://localhost:8080) in your browser. The C++ server will serve `index.html`, `styles.css`, and `app.js` directly!
2. **Direct Browser File**:
   Double click [**`index.html`**](file:///C:/Users/user/.gemini/antigravity/scratch/ecommerce-storefront/index.html). The frontend will auto-detect the local C++ backend API on port 8080 and display `🟢 C++ Server Online`.

---

## 🗄️ Database Features

The backend features a disk-persisted database engine stored in `backend/data/`:
- **Thread-Safe**: Uses native Windows `CRITICAL_SECTION` mutex locks (`LockGuard`) for concurrent read/write protection.
- **Auto-Seeding**: Automatically seeds the 10-item curated catalogue and initial discount codes (`SPRING20`, `AURA10`, `FREESHIP`) on first launch if empty.
- **Transactions & Orders**: Every checkout is assigned a unique confirmation ID (`#AURA-#####`), timestamped, and immediately flushed to `data/orders.json`.
- **Coupon Usage Tracking**: Tracking how many times each coupon is redeemed in `data/coupons.json`.

---

## 📡 REST API Reference

Base URL: `http://localhost:8080/api`

| Method | Endpoint | Description |
| :--- | :--- | :--- |
| `GET` | `/api/health` | Health check, server version, and uptime timestamp |
| `GET` | `/api/products` | Retrieve all products (Supports `?category=`, `?search=`, `?maxPrice=`, `?sort=`) |
| `GET` | `/api/products/:id` | Retrieve single product details by ID |
| `POST` | `/api/products` | Insert a new product into the database |
| `GET` | `/api/orders` | Retrieve list of all placed customer orders |
| `POST` | `/api/orders` | Submit a new order (saves customer info, items, and totals to database) |
| `GET` | `/api/orders/:id` | Retrieve single order confirmation receipt |
| `POST` | `/api/coupons/validate` | Check validity of coupon code (`{"code":"SPRING20"}`) |
| `GET` | `/api/stats` | View sales metrics (total revenue, order count, product count) |
| `GET` | `/*` | Static file server for HTML, CSS, JS, and image assets |

---

## 🛠️ Manual Compilation Command

If you want to manually recompile the C++ server using `g++` (MinGW):

```powershell
g++ -std=c++14 -O2 main.cpp database.cpp http_server.cpp -lws2_32 -o bin\server.exe
```
